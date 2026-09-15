#include "input-policy.hpp"

#include <QCursor>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QImage>
#include <QMutex>
#include <QMutexLocker>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickView>
#include <QRegion>
#include <QScreen>
#include <QStandardPaths>
#include <QTimer>
#include <QVulkanInstance>
#include <windows.h>
#include <atomic>

using Qt::StringLiterals::operator""_s;
using caelestia::windows::InputPolicy;

namespace {
QFile logFile;
std::atomic_bool hadWarning = false;
QMutex logMutex;

void logMessage(QtMsgType type, const QMessageLogContext&, const QString& message) {
    const QMutexLocker lock(&logMutex);
    if (type == QtWarningMsg || type == QtCriticalMsg || type == QtFatalMsg)
        hadWarning = true;
    logFile.write(message.toUtf8() + '\n');
    logFile.flush();
}

class TestView final : public QQuickView {
protected:
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override {
        const auto* msg = static_cast<MSG*>(message);
        if (msg->message == WM_HOTKEY && msg->wParam == 1) {
            QGuiApplication::quit();
            *result = 0;
            return true;
        }
        return QQuickView::nativeEvent(eventType, message, result);
    }
};

bool setClickThrough(HWND window, bool enabled) {
    SetLastError(0);
    const LONG_PTR style = GetWindowLongPtrW(window, GWL_EXSTYLE);
    if (!style && GetLastError())
        return false;
    const LONG_PTR next = enabled ? style | WS_EX_LAYERED | WS_EX_TRANSPARENT : style & ~WS_EX_TRANSPARENT;
    SetLastError(0);
    if (!SetWindowLongPtrW(window, GWL_EXSTYLE, next) && GetLastError())
        return false;
    return SetLayeredWindowAttributes(window, 0, 255, LWA_ALPHA) != 0;
}
}

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName(u"Caelestia First Test"_s);
    QStandardPaths::setTestModeEnabled(true);
    const bool inputTarget = app.arguments().contains(u"--input-target"_s);
    const QString package = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(u".."_s);
    logFile.setFileName(package + (inputTarget ? u"/input-target.log"_s : u"/first-test.log"_s));
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return 2;
    qInstallMessageHandler(logMessage);

    const auto args = app.arguments();
    const bool smoke = args.contains(u"--smoke-test"_s);
    const bool overlay = !args.contains(u"--preview"_s) && !smoke && !inputTarget;
    const bool d3d = !args.contains(u"--vulkan"_s);
    const bool tightMask = args.contains(u"--tight-mask"_s);
    const bool paddedMask = args.contains(u"--padded-mask"_s);
    QVulkanInstance vulkan;
    if (d3d) {
        QQuickWindow::setGraphicsApi(QSGRendererInterface::Direct3D11);
    } else {
        QQuickWindow::setGraphicsApi(QSGRendererInterface::Vulkan);
        if (!vulkan.create()) {
            qCritical() << "Vulkan instance failed:" << vulkan.errorCode();
            return 3;
        }
    }

    QQuickWindow::setDefaultAlphaBuffer(true);
    TestView view;
    if (!d3d)
        view.setVulkanInstance(&vulkan);
    view.setTitle(u"Caelestia — renderer/input test"_s);
    view.setColor(Qt::transparent);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    if (overlay) {
        view.setFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus);
        view.setGeometry(app.primaryScreen()->availableGeometry());
    } else {
        if (inputTarget) {
            view.setTitle(u"Caelestia click-through target"_s);
            view.setGeometry(300, 500, 480, 280);
        } else {
            view.resize(1280, 720);
        }
    }
    view.engine()->addImportPath(package + u"/qml"_s);
    view.rootContext()->setContextProperty(u"testMode"_s,
        tightMask ? u"tight rendering mask"_s : paddedMask ? u"padded rendering mask"_s : u"cursor-based click-through"_s);
    QObject::connect(view.engine(), &QQmlEngine::quit, &app, &QGuiApplication::quit);
    view.setSource(QUrl::fromLocalFile(package + (inputTarget
        ? u"/qml/FirstTest/InputTarget.qml"_s : u"/qml/FirstTest/Main.qml"_s)));
    if (view.status() != QQuickView::Ready) {
        qCritical() << "Scene failed to load" << view.errors();
        return 4;
    }
    view.show();
    qInfo() << "Surface alpha bits:" << view.format().alphaBufferSize();
    if (inputTarget)
        return app.exec();
    const HWND hwnd = reinterpret_cast<HWND>(view.winId());
    if (overlay && !RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 'Q')) {
        qCritical() << "Cannot register Ctrl+Alt+Q exit key:" << GetLastError();
        return 5;
    }
    auto* root = view.rootObject();
    bool through = false;
    bool escapeHeld = false;
    bool suppressed = false;
    QTimer input;
    input.setInterval(8);
    QObject::connect(&input, &QTimer::timeout, &app, [&] {
        const QPoint point = view.mapFromGlobal(QCursor::pos());
        const InputPolicy policy{double(view.width()), double(view.height()),
            root->property("barWidth").toDouble(), root->property("borderThickness").toDouble(),
            root->property("drawerWidth").toDouble(), root->property("drawerHeight").toDouble()};
        const double progress = root->property("progress").toDouble();
        const bool escape = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;
        if (escape && !escapeHeld)
            suppressed = true;
        escapeHeld = escape;
        if (!policy.keepOpen(point.x(), point.y(), 1.0))
            suppressed = false;
        const bool dragging = ((GetAsyncKeyState(VK_LBUTTON) | GetAsyncKeyState(VK_RBUTTON)) & 0x8000) != 0;
        if (!smoke && !dragging) {
            const bool isOpen = root->property("targetOpen").toBool();
            root->setProperty("targetOpen", !suppressed && (isOpen
                ? policy.keepOpen(point.x(), point.y(), progress) : policy.inTrigger(point.x(), point.y())));
        }
        if (!overlay)
            return;
        if (tightMask || paddedMask) {
            const int padding = paddedMask ? 20 : 0;
            QRegion region(0, 0, view.width(), view.height());
            region -= QRegion(int(policy.barWidth), int(policy.border),
                view.width() - int(policy.barWidth + policy.border), view.height() - int(2 * policy.border));
            region += QRegion(int(policy.drawerLeft()) - padding, 0,
                int(policy.drawerWidth) + 2 * padding, int(policy.border + policy.drawerHeight * std::clamp(progress, 0.0, 1.0)) + padding);
            view.setMask(region);
        } else if (!dragging) {
            const bool next = !policy.intercepts(point.x(), point.y(), progress);
            if (next != through) {
                if (!setClickThrough(hwnd, next)) {
                    qCritical() << "Click-through update failed:" << GetLastError();
                    app.exit(6);
                }
                through = next;
            }
        }
    });
    input.start();

    if (smoke) {
        root->setProperty("exclusionTest", true);
        root->setProperty("targetOpen", true);
        QTimer::singleShot(1200, &app, [&] {
            const auto frame = view.grabWindow();
            const bool animated = root->property("progress").toDouble() > 0.99;
            const bool rendered = !frame.isNull() && frame.pixelColor(5, 5).alpha() > 0
                && frame.pixelColor(frame.width() / 2, 100).alpha() > 0;
            const bool excluded = !frame.isNull() && frame.pixelColor(250, 550).alpha() > 200
                && frame.pixelColor(305, 550).alpha() < 80;
            const bool saved = frame.save(package + u"/smoke-open.png"_s);
            if (!animated || !rendered || !excluded || !saved || hadWarning) {
                qCritical() << "Smoke failed" << animated << rendered << excluded << saved << hadWarning;
                app.exit(7);
                return;
            }
            root->setProperty("exclusionsEnabled", false);
            QTimer::singleShot(150, &app, [&] {
                const auto mergedFrame = view.grabWindow();
                const int mergedAlpha = mergedFrame.isNull() ? 0 : mergedFrame.pixelColor(305, 550).alpha();
                if (mergedAlpha <= 80) {
                    qCritical() << "Excluded-pair control did not merge:" << mergedAlpha;
                    app.exit(9);
                    return;
                }
                qInfo() << "PASS: excluded gap alpha < 80; non-excluded control alpha" << mergedAlpha;
                root->setProperty("targetOpen", false);
                QTimer::singleShot(900, &app, [&] {
                    if (root->property("progress").toDouble() > 0.01 || hadWarning) {
                        app.exit(8);
                        return;
                    }
                    qInfo() << "PASS: QML load, animated drawer, rendered frame and clean shutdown";
                    app.quit();
                });
            });
        });
    }
    const int result = app.exec();
    if (overlay)
        UnregisterHotKey(hwnd, 1);
    return result;
}
