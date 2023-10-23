# Optional
Моя реализация класса стандартной библиотеки std::optional
***

## Развертывание
```
g++ main.cpp optional.h -o optional -std=c++17 -O3
./optional
```
# Формат выходных данных
```
HasValue();            Проверяет есть ли значение
Value();               Получает значение
```  
## Использование
### Ввод
```
int main() {
        std::string value = "has value";
        Optional<int> o(std::move(value));
        assert(o.HasValue());
        std::cout << o.Value();
}
```
### Вывод
```
has value
```

