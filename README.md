# MyCppPlaner

Прототип календаря-планировщика на C++ и Qt Widgets. Первый проект на C++:
разобраться с CMake, Qt и сборкой десктопного приложения.

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

## Статус

Ранний прототип. Реализован только календарь; регистрация и хранение событий —
заготовки. Проект ценен как первый опыт с C++/Qt/CMake, а не как готовый продукт.

## Что бы улучшил

- Добавить модель событий и хранение в SQLite.
- Сделать форму добавления события.
- Убрать хардкод пути к Qt через `CMAKE_PREFIX_PATH` из окружения.
- Добавить скриншот окна в README.
