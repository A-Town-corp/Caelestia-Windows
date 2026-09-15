#pragma once

#include <algorithm>

namespace caelestia::windows {

// Logical (DPI-independent) coordinates, relative to one monitor's overlay.
struct InputPolicy {
    double width;
    double height;
    double barHeight;
    double border;
    double drawerWidth;
    double drawerHeight;

    [[nodiscard]] double drawerLeft() const {
        return (width - drawerWidth) / 2;
    }

    [[nodiscard]] bool inScreen(double x, double y) const {
        return x >= 0 && y >= 0 && x < width && y < height;
    }

    [[nodiscard]] bool inTrigger(double x, double y) const {
        return inScreen(x, y) && x >= drawerLeft() && x < drawerLeft() + drawerWidth && y < border;
    }

    [[nodiscard]] bool keepOpen(double x, double y, double progress) const {
        const double visibleHeight = std::clamp(progress, 0.0, 1.0) * drawerHeight;
        return inScreen(x, y) && x >= drawerLeft() && x < drawerLeft() + drawerWidth
            && y < border + visibleHeight;
    }

    [[nodiscard]] bool intercepts(double x, double y, double progress) const {
        return inScreen(x, y) && (x < border || x >= width - border || y < border
            || y >= height - barHeight || keepOpen(x, y, progress));
    }
};

} // namespace caelestia::windows
