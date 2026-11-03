# MyCppPlaner

Календарь-планировщик на C++ и Qt Widgets. Первый заход в C++: CMake, Qt Widgets
и сборка десктопного приложения от пустого проекта.

## Стек

- C++17
- Qt 6 (Core, Widgets)
- CMake

## Что реализовано

- Главное окно с `QCalendarWidget`.
- Заготовка окна регистрации.
- Сборка через CMake с `AUTOMOC`, `AUTORCC`, `AUTOUIC`.
- Разделение на `include/` и `src/`.

## Как собрать

1. Установи Qt 6 (например, через Qt Online Installer).
2. Поправь путь к Qt в `CMakeLists.txt`:

```cmake
set(CMAKE_PREFIX_PATH "C:/Program Files/Qt/6.5.2/msvc2019_64")
```

3. Собери:

```bash
cmake -B build -S .
cmake --build build --config Release
```

4. Запусти `MyCppPlanner.exe` из `build`.

## Состояние

Каркас приложения готов: главное окно с календарём, структура `include/` + `src/`,
сборка через CMake с `AUTOMOC`/`AUTORCC`/`AUTOUIC`. Регистрация и хранение
событий пока заготовки — это следующая итерация.

## Дальнейшие шаги

- Добавить модель событий и хранение в SQLite.
- Сделать форму добавления события.
- Убрать хардкод пути к Qt через `CMAKE_PREFIX_PATH` из окружения.
- Добавить скриншот окна в README.
