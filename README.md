# 2Q and Perfect cache 
# Установка и сборка
Клонируйте репозиторий, перейдите в папку с ним.
Сборка проекта:
> Note: `<Результат сборки будет лежать в в поддиректории проекта build*`
```sh
CXX=clang++ cmake -B build
```
> Note: `<Сборка другим компилятором возможно приведет к ошибке компиляции явной спецификации функции шаблонного класса, в стандарте начиная с с++17 это разрешено, но есть группа компиляторов которые это не поддерживают (в частности g++)"An explicit specialization ... may be declared in any scope in which the corresponding primary template may be defined (_N4868_.9.8.2.3 [namespace.memdef], 11.4 [class.mem], 13.7.3 [temp.mem])."*`

# Исползование 
## Тесты :
Запуск unit тестов происходит через Google Test. 
> Note: `<Все необходимые файлы для работы с Google Test автоматически добавятся к проекту.>*`
Все тесты лежат в директории репозитория в папке "tests". 
Для запуска unit тестов директории build напишите "ctest"

запуск end-to-end тестов:
> Note: `<end-to-end тесты лежат в поддректоии проекта /tests.>*`
```sh
cd tests/
./tests.sh*
```
## 2Q cache
Чтобы запустить 2Q cache :
> Note: `<Результат сборки лежит в build/cache_all.>*`
```sh
cd cache_all/
./cache <../../test/*
```
Вместо *нужно указать имя файла с вашим тестом
К примеру :
```sh
./lirs ../../tests/data/tasks/test_01.dat
```
## Perfect_cache
Чтобы запустить perfect_cache (реализация алгоритма Belady):
> Note: `<Результат сборки лежит в build/cache_all.>*`
```sh
cd cache_all/
./cache <../../test/*
> Note: `Аналогично предыдущему с *````