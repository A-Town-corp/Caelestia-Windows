#include "../src/input-policy.hpp"

#include <iostream>

int main() {
    using caelestia::windows::InputPolicy;
    const InputPolicy policy{2560, 1440, 60, 10, 854, 340};
    int checks = 0;
    auto expect = [&checks](bool condition, const char* name) {
        ++checks;
        if (!condition) {
            std::cerr << "FAIL: " << name << '\n';
            return false;
        }
        return true;
    };
    bool ok = true;
    ok &= expect(policy.inTrigger(1310, 0), "top edge opens dashboard");
    ok &= expect(!policy.inTrigger(20, 0), "unrelated top edge does not open dashboard");
    ok &= expect(!policy.inTrigger(1310, -1), "other monitor does not open dashboard");
    ok &= expect(!policy.inTrigger(1310, 10), "trigger has an exclusive lower bound");
    ok &= expect(policy.intercepts(1310, 120, 1.0), "open drawer receives clicks");
    ok &= expect(!policy.intercepts(1310, 120, 0.0), "closed drawer releases desktop");
    ok &= expect(policy.intercepts(1310, 80, 0.5), "animated drawer receives clicks");
    ok &= expect(!policy.intercepts(1310, 250, 0.5), "space below animated drawer passes through");
    ok &= expect(policy.intercepts(4, 600, 0.0), "thin left border receives clicks");
    ok &= expect(!policy.intercepts(30, 600, 0.0), "former left bar releases desktop");
    ok &= expect(policy.intercepts(400, 1380, 0.0), "bottom bar includes its top edge");
    ok &= expect(!policy.intercepts(400, 1379, 0.0), "desktop above bottom bar passes through");
    ok &= expect(policy.intercepts(400, 1400, 0.0), "bottom bar receives clicks");
    ok &= expect(policy.drawerLeft() == 853, "top drawer is centred on screen");
    ok &= expect(policy.inTrigger(853, 0), "centred trigger includes left edge");
    ok &= expect(!policy.inTrigger(1707, 0), "centred trigger excludes right edge");
    ok &= expect(!policy.intercepts(400, 600, 0.0), "desktop receives clicks");
    ok &= expect(!policy.intercepts(2560, 20, 1.0), "right boundary is excluded");
    ok &= expect(!policy.intercepts(100, 1440, 1.0), "bottom boundary is excluded");
    ok &= expect(policy.keepOpen(1310, 200, 1.0), "pointer can travel into drawer");
    ok &= expect(!policy.keepOpen(300, 500, 1.0), "leaving drawer closes it");
    ok &= expect(policy.keepOpen(1310, 0, 0.0), "trigger bridges the closed state");
    ok &= expect(!policy.intercepts(1310, 370, 1.2), "animation overshoot cannot grow input beyond drawer");
    if (!ok)
        return 1;
    std::cout << "PASS: " << checks << " input-policy checks\n";
}
