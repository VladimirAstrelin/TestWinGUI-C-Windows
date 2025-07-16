#include <windows.h>  // Подключаем основной заголовочный файл Windows API
#include "resource.h" // Подключаем сгенерированный файл с идентификаторами ресурсов

// Глобальные переменные для хранения состояния элементов:
BOOL g_bCheck1 = FALSE;  // Состояние первого чекбокса (выключен по умолчанию)
BOOL g_bCheck2 = FALSE;  // Состояние второго чекбокса
int g_nRadio = 1;        // Текущая выбранная радио-кнопка (1 = IDC_RADIO1, 2 = IDC_RADIO2)
WCHAR g_szEditText[256] = L""; // Буфер для текста из поля ввода (Unicode-строка)

// Основная процедура обработки сообщений диалогового окна:
INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {  // Анализируем тип полученного сообщения
    case WM_INITDIALOG:  // Сообщение инициализации диалога (первое сообщение)
        // Заполняем список (ListBox) элементами:
        SendDlgItemMessage(hwnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)L"Элемент списка 1");
        // Параметры: 
        // hwnd - handle окна, 
        // IDC_LIST1 - ID списка,
        // LB_ADDSTRING - команда добавления строки,
        // 0 - неиспользуемый параметр,
        // (LPARAM)L"..." - добавляемая строка

        SendDlgItemMessage(hwnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)L"Элемент списка 2");
        SendDlgItemMessage(hwnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)L"Элемент списка 3");

        // Заполняем выпадающий список (ComboBox):
        SendDlgItemMessage(hwnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"Вариант 1");
        SendDlgItemMessage(hwnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"Вариант 2");
        SendDlgItemMessage(hwnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)L"Вариант 3");
        SendDlgItemMessage(hwnd, IDC_COMBO1, CB_SETCURSEL, 0, 0); // Выбираем первый элемент (индекс 0)

        // Устанавливаем начальное состояние радио-кнопок:
        CheckDlgButton(hwnd, IDC_RADIO1, BST_CHECKED); // IDC_RADIO1 включена

        // Устанавливаем текст по умолчанию для поля ввода:
        SetDlgItemText(hwnd, IDC_EDIT1, L"Введите текст здесь");
        // Параметры:
        // hwnd - handle окна,
        // IDC_EDIT1 - ID поля ввода,
        // L"..." - текст для установки

        return TRUE;  // Возвращаем TRUE, чтобы система установила фокус

    case WM_COMMAND:  // Обработка команд от элементов управления
        switch (LOWORD(wParam)) {  // LOWORD(wParam) содержит ID элемента
        case IDOK:  // Обработка кнопки OK
        {
            // Получаем состояние чекбоксов:
            g_bCheck1 = IsDlgButtonChecked(hwnd, IDC_CHECK1) == BST_CHECKED;
            g_bCheck2 = IsDlgButtonChecked(hwnd, IDC_CHECK2) == BST_CHECKED;

            // Определяем какая радио-кнопка выбрана:
            g_nRadio = IsDlgButtonChecked(hwnd, IDC_RADIO1) ? 1 : 2;

            // Получаем текст из поля ввода:
            GetDlgItemText(hwnd, IDC_EDIT1, g_szEditText, 256);

            // Получаем выбранный элемент списка:
            LRESULT listIndex = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
            WCHAR listText[256] = L"";  // Буфер для текста элемента
            if (listIndex != LB_ERR) {  // Если элемент выбран
                SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, (WPARAM)listIndex, (LPARAM)listText);
            }

            // Получаем выбранный элемент комбобокса:
            LRESULT comboIndex = SendDlgItemMessage(hwnd, IDC_COMBO1, CB_GETCURSEL, 0, 0);
            WCHAR comboText[256] = L"";
            if (comboIndex != CB_ERR) {
                SendDlgItemMessage(hwnd, IDC_COMBO1, CB_GETLBTEXT, (WPARAM)comboIndex, (LPARAM)comboText);
            }

            // Формируем информационное сообщение:
            WCHAR infoMsg[1024];  // Буфер для сообщения
            wsprintf(infoMsg,      // Аналог sprintf для Unicode
                L"Состояние элементов:\n"
                L"- Check1: %s\n"
                L"- Check2: %s\n"
                L"- Radio: %d\n"
                L"- Текст: %s\n"
                L"- Выбранный элемент списка: %s\n"
                L"- Выбранный вариант комбобокса: %s",
                g_bCheck1 ? L"Вкл" : L"Выкл",  // Тернарный оператор для состояния чекбокса
                g_bCheck2 ? L"Вкл" : L"Выкл",
                g_nRadio,                     // Номер радио-кнопки
                g_szEditText,                 // Текст из Edit
                listIndex != LB_ERR ? listText : L"Ничего не выбрано",
                comboIndex != CB_ERR ? comboText : L"Ничего не выбрано");

            // Показываем сообщение с собранной информацией:
            MessageBox(hwnd, infoMsg, L"Итоговая информация", MB_OK);
            // Параметры:
            // hwnd - родительское окно,
            // infoMsg - текст сообщения,
            // L"..." - заголовок окна,
            // MB_OK - тип кнопок (OK)

            // Закрываем диалоговое окно с результатом TRUE:
            EndDialog(hwnd, TRUE);
            return TRUE;
        }

        case IDCANCEL:  // Обработка кнопки Cancel
            EndDialog(hwnd, FALSE);  // Закрываем диалог с результатом FALSE
            return TRUE;

        case IDC_CHECK1:  // Обработка первого чекбокса
            MessageBox(hwnd, IsDlgButtonChecked(hwnd, IDC_CHECK1) ?
                L"Чекбокс 1 включен" : L"Чекбокс 1 выключен",
                L"Информация", MB_OK);
            return TRUE;

        case IDC_CHECK2:  // Обработка второго чекбокса
            MessageBox(hwnd, IsDlgButtonChecked(hwnd, IDC_CHECK2) ?
                L"Чекбокс 2 включен" : L"Чекбокс 2 выключен",
                L"Информация", MB_OK);
            return TRUE;

        case IDC_RADIO1:  // Обработка радио-кнопок
        case IDC_RADIO2:
            MessageBox(hwnd,
                IsDlgButtonChecked(hwnd, IDC_RADIO1) ?
                L"Выбрана радио-кнопка 1" : L"Выбрана радио-кнопка 2",
                L"Информация", MB_OK);
            return TRUE;

        case IDC_BUTTON1:  // Обработка обычной кнопки
        {
            WCHAR text[256] = { 0 };  // Буфер для текста
            GetDlgItemText(hwnd, IDC_EDIT1, text, 256);  // Получаем текст из Edit
            MessageBox(hwnd, text, L"Текст из поля ввода", MB_OK);  // Показываем текст
        }
        return TRUE;

        case IDC_LIST1:  // Обработка списка
            if (HIWORD(wParam) == LBN_DBLCLK) {  // Если было двойное нажатие
                LRESULT index = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
                if (index != LB_ERR) {  // Если элемент выбран
                    WCHAR text[256] = { 0 };  // Буфер с инициализацией нулями
                    SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, (WPARAM)index, (LPARAM)text);
                    MessageBox(hwnd, text, L"Выбранный элемент списка", MB_OK);
                }
            }
            return TRUE;

        case IDC_COMBO1:  // Обработка комбобокса
            if (HIWORD(wParam) == CBN_SELCHANGE) {  // Если изменился выбор
                LRESULT index = SendDlgItemMessage(hwnd, IDC_COMBO1, CB_GETCURSEL, 0, 0);
                if (index != CB_ERR) {
                    WCHAR text[256] = { 0 };
                    SendDlgItemMessage(hwnd, IDC_COMBO1, CB_GETLBTEXT, (WPARAM)index, (LPARAM)text);
                    MessageBox(hwnd, text, L"Выбранный вариант", MB_OK);
                }
            }
            return TRUE;
        }
        break;
    }
    return FALSE;  // Возвращаем FALSE для неперехваченных сообщений
}

// Точка входа Windows-приложения:
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,      // Дескриптор экземпляра приложения
    _In_opt_ HINSTANCE hPrevInstance, // Не используется (всегда NULL)
    _In_ LPSTR lpCmdLine,          // Аргументы командной строки (ANSI)
    _In_ int nCmdShow)             // Параметры отображения окна
{
    // Создаем и отображаем модальное диалоговое окно:
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
    // Параметры:
    // hInstance - дескриптор приложения,
    // MAKEINTRESOURCE(IDD_DIALOG1) - ID диалога,
    // NULL - родительское окно,
    // DialogProc - функция обработки сообщений

    return 0;  // Выход из приложения
}


