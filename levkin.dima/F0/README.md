# REUPTAKE LAYOUT

## Файлы 
### Файл типа 1: layouts.db (Глобальный индекс)
Хранит пары: Название_Маккета Путь_К_Файлу_Дерева.

```Plaintext
main_page layouts/main.rl
dashboard layouts/dash.rl
auth_form layouts/auth.rl
```

### Файл типа 2: .rl (Файл дерева макета)
Строка формата:
parent_id id width height flexDirection justify align
Если это корень, у него нет родителя: null.

```Plaintext
null root 1920.0 1080.0 Column FlexStart FlexStart
root header 1920.0 80.0 Row SpaceBetween Center
root content -1.0 -1.0 Row FlexStart FlexStart
content sidebar 300.0 -1.0 Column FlexStart FlexStart
```