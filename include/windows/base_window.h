#pragma once

#include <functional>
#include <unordered_map>
#include <variant>
#include <string>
#include <QString>

namespace std {
    template <>
    struct hash<WindowType> {
        size_t operator()(WindowType type) const { // «р€ ты сюда полез..
            return static_cast<size_t>(type);
        }
    };
}

enum class WindowType {
    Login,
    Main,
    Settings,
    TaskDetails,
    Calendar,
    Basic
};

using WindowParamsArgs = std::variant<int, QString, bool, WindowType>;
using WindowParams = std::unordered_map<std::string, WindowParamsArgs>;

struct Error {
    std::string message;
};

std::variant<WindowParamsArgs, Error> getStoredInfoFromParamsByKey(const std::string& key, const WindowParams& params) {
    auto findIt = params.find(key);
    if (findIt != params.end()) {
        return findIt->second;
    }
    return Error{ "no such key" };
}

class BaseWindow : public QWidget {
    Q_OBJECT

public:
    explicit BaseWindow(WindowParams params, QWidget* parent = nullptr)
        : QWidget(parent), type(WindowType::Basic) {
    }

    virtual ~BaseWindow() = default;

    WindowType type;
    virtual void initialize(const WindowParams& params) = 0;

protected:
    virtual void setupUI() = 0;
    virtual void cleanup() {}
    virtual void showNextWindow(WindowParams params) { Q_UNUSED(type); }
    virtual void goBack() {}
    virtual void goHome() {}
};