#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <ctype.h>
#include <windows.h>
#include <commdlg.h>

#pragma warning(disable:4996)

#define RED   "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,!?;:()[]{}\"'\\|/…@#$%&*_-+= ";    // кількість символів (включно з термінатором закінчення, і двома \ \ через символи)

// список вживаних слів
const char* commonWords[] = {
"accept", "accomplish", "act", "add", "adapt", "affect", "agree", "aim", "analyze", "announce", "answer",
"appear", "appreciate", "approve", "argue", "arrange", "arrive", "ask", "assume", "attack", "authorize",
"avoid", "believe", "begin", "break", "bring", "build", "buy", "calculate", "call", "care", "challenge",
"change", "choose", "clarify", "clean", "clear", "collect", "come", "command", "communicate", "complain",
"complete", "confirm", "connect", "consider", "contain", "continue", "contribute", "control", "convince",
"create", "criticize", "decide", "declare", "decrease", "define", "delete", "deliver", "demand", "deny",
"depend", "describe", "design", "develop", "discover", "discuss", "dislike", "dismiss", "dispute", "divide",
"do", "donate", "drink", "dream", "earn", "educate", "effect", "eliminate", "emphasize", "enjoy", "ensure",
"enter", "erase", "establish", "estimate", "evaluate", "examine", "exceed", "exclude", "exist", "expect",
"explain", "explore", "express", "extend", "fail", "fall", "feel", "find", "follow", "forget", "forgive",
"freeze", "gain", "get", "give", "go", "grant", "grow", "guess", "handle", "hate", "hear", "help", "hide",
"hold", "hope", "ignore", "impact", "implement", "improve", "include", "increase", "influence", "inform",
"inherit", "injure", "inspect", "inspire", "install", "instruct", "interpret", "introduce", "invent",
"invite", "involve", "justify", "keep", "know", "learn", "leave", "let", "like", "limit", "listen", "live",
"look", "lose", "love", "maintain", "make", "manage", "measure", "meet", "mention", "modify", "motivate",
"move", "need", "negotiate", "notice", "observe", "offer", "oppose", "order", "organize", "overcome",
"own", "participate", "permit", "persuade", "plan", "play", "predict", "prefer", "prepare", "preserve",
"prevent", "produce", "progress", "prohibit", "promise", "propose", "protect", "prove", "provide", "pull",
"punish", "purchase", "push", "question", "raise", "react", "realize", "receive", "recognize", "recommend",
"record", "recover", "reduce", "refuse", "regulate", "reject", "relate", "remember", "remind", "remove",
"repair", "replace", "report", "represent", "require", "research", "reserve", "resist", "resolve",
"respect", "respond", "restrict", "result", "retain", "return", "reveal", "review", "ride", "rise",
"save", "say", "schedule", "search", "secure", "see", "seek", "separate", "serve", "set", "share",
"show", "succeed", "suffer", "suggest", "support", "suppose", "survive", "take", "teach", "tell",
"think", "try", "understand", "use", "value", "verify", "wait", "walk", "want", "warn", "watch",
"win", "wish", "work", "write"
};

// Функція для знаходження мультиплікативно оберненого
int mod_inverse(int a) {
  int i;
  for (i = 1; i < alphabet.size(); i++) {
    if ((a * i) % alphabet.size() == 1)
      return i;
  }
  return -1; // Якщо обернене число не знайдено
}

// Функція перевірки чи число є простим
bool is_simple(int n) {
  int i;

  if (n < 2) 
    return false;

  for (i = 2; i < n; i++) {
    if (n % i == 0)
      return false;
  }
  return true;
}

// Функція шифрування
void affine_encrypt(const char* plaintext, char* ciphertext, int a, int b) {
  int i, x, y;

  for (i = 0; i < strlen(plaintext); i++) {
    const char* found = strchr(alphabet.c_str(), plaintext[i]);   // strchr - шукає символ у алфавіті, результат функції - адреса памяті де знаходиться перший такий смвол
    if (found) {
      x = found - alphabet.c_str();       // Позиція символу у алфавіті
      y = (a * x + b) % alphabet.size();
      ciphertext[i] = alphabet[y]; // Зашифрований символ
    }
    else {
      ciphertext[i] = plaintext[i]; // Залишаємо символ без змін
    }
  }
  ciphertext[strlen(plaintext)] = '\0'; // Додаємо завершення рядка
}

// Функція дешифрування
void affine_decrypt(const char* ciphertext, char* plaintext, int a, int b) {
  int n = alphabet.size(),
    a_inverse = mod_inverse(a),
    y,        // Індекс символу у алфавіті (для зашифрованого повідомлення)
    x,        // Індекс символу у алфавіті (для розшифрованого повідомлення)
    i;        // Індекс символу у введеній строці

  for (i = 0; i < strlen(ciphertext); i++) {
    y = alphabet.find(ciphertext[i]); // Знаходимо позицію символу в алфавіті
    if (y != std::string::npos) { // Якщо символ знайдено
      x = (a_inverse * ((y - b + n) % n)) % n; // Коректне обчислення x
      x = (x + n) % n;      // Для таких випадків як a = 3, b = 2
      plaintext[i] = alphabet[x]; // Дешифрування
    }
    else {
      plaintext[i] = ciphertext[i]; // Якщо символ не знайдено, залишаємо без змін
    }
  }

  plaintext[strlen(ciphertext)] = '\0';
}

// Функція запису у файл
void writeToFile(const char* filename, const char* message, const char* mode) {
  FILE* file = fopen(filename, mode);
  if (file == NULL) {
    printf("Помилка відкриття файлу для запису!\n");
    return;
  }

  fprintf(file, "%d %s", strlen(message), message);
  fclose(file);
}

// Функція читання файлу
void readFromFile(const char* filename, char* message) {
  int size = 0;

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("Помилка відкриття файлу для зчитування!\n");
    return;
  }

  fscanf(file, "%d ", &size);
  fgets(message, size + 1, file);
  fclose(file);
}

// Функція вибору файлу
char* OpenFileDialog() {
  static wchar_t szFile[MAX_PATH] = { 0 }; // Буфер для обраного файлу
  static char filePath[MAX_PATH] = { 0 }; // Буфер для конвертованого шляху в char
  OPENFILENAMEW ofn;

  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFilter = L"Text Files\0*.TXT\0";
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

  if (GetOpenFileName(&ofn)) {
    WideCharToMultiByte(CP_UTF8, 0, szFile, -1, filePath, MAX_PATH, NULL, NULL);
    printf("Обраний файл: %s\n", filePath);
    return filePath;
  }
  else {
    printf("Вибір файлу скасовано користувачем.\n");
    return NULL;
  }
}

// Функція запиту на збереження файлу
void askToSaveToFile(const char* message) {
  int save_inFile;

  printf("\nЗаписати у файл (0 / 1): ");
  scanf("%d", &save_inFile);
  if (save_inFile == 1) {
    printf("\nОберіть файл, куди записати\n");
    const char* filename = OpenFileDialog();
    if (!filename) return;
    writeToFile(filename, message, "w");
  }
}

// Функція запису користувачем параметрів шифрування
void inputCryptingParam(int* a, int* b) {
  do {
    printf("Введіть простий множник (0 < a < %d): ", alphabet.size() + 1);
    scanf("%d", a);
    if (*a <= 0 || *a > alphabet.size())
      printf("Число за межами\n");
    else if (!is_simple(*a))
      printf("Число не просте, має бути простим\n");
    else { ; }
  } while (!is_simple(*a) || (*a <= 0 || *a > alphabet.size()));

  printf("Введіть зсув (0 < b < %d): ", alphabet.size() + 1);
  scanf("%d", b);
}

// Функція шифрування консоллю
void console_encrypting() {
  int a, b, n;
  char plaintext[1024];         // масив для збереження введеного тексту до шифрування
  char ciphertext[1024];       // масив для збереження зашифрованого тексту

  printf("Усього символів: %d\n", alphabet.size());
  printf("Введіть текст для шифрування: ");
  fgets(plaintext, sizeof(plaintext), stdin);

  plaintext[strcspn(plaintext, "\n")] = '\0'; // Видалення символу нового рядка


  inputCryptingParam(&a, &b);

  affine_encrypt(plaintext, ciphertext, a, b);
  printf("Зашифрований текст: %s%s%s\n", GREEN, ciphertext, RESET);

  askToSaveToFile(ciphertext);
};

// Функція дешифрування консоллю
void console_decrypting() {
  int a, b, n;
  char ciphertext[1024];       // масив для збереження зашифрованого тексту
  char decryptedtext[1024];   // масив для збереження розшифрованого тексту

  printf("Усього символів: %d\n", alphabet.size());
  printf("Введіть текст для шифрування: ");
  fgets(ciphertext, sizeof(ciphertext), stdin);

  ciphertext[strcspn(ciphertext, "\n")] = '\0'; // Видалення символу нового рядка

  inputCryptingParam(&a, &b);

  affine_decrypt(ciphertext, decryptedtext, a, b);
  printf("Розшифрований текст: %s%s%s\n", GREEN, decryptedtext, RESET);

  askToSaveToFile(decryptedtext);
};

// Функція шифрування файлу
void file_encrypting() {
  int a, b, save_inFile;
  char ciphertext[1024],       // Масив для збереження зашифрованого тексту
    plaintext[1024];          // Введенний текст

  const char* filename_read;


  filename_read = OpenFileDialog();
  if (!filename_read) return;
  readFromFile(filename_read, plaintext);

  inputCryptingParam(&a, &b);

  affine_encrypt(plaintext, ciphertext, a, b);
  printf("Зашифрований текст: %s%s%s\n", GREEN, ciphertext, RESET);

  askToSaveToFile(ciphertext);
};

// Функція дешифрування файлу
void file_decrypting() {
  int a, b;
  char ciphertext[1024],
    plaintext[1024];

  const char* filename_read;

  filename_read = OpenFileDialog();
  if (!filename_read) return;
  readFromFile(filename_read, ciphertext);

  inputCryptingParam(&a, &b);

  affine_decrypt(ciphertext, plaintext, a, b);
  printf("Розшифрований текст: %s%s%s\n", GREEN, plaintext, RESET);

  askToSaveToFile(plaintext);
};

// Функція Brute Force
void bruteForce() {
  const char* filename_read;
  const char* filename_write;
  char encryptedText[1024],
    text_toLower[1024];

  int a, b, i, j;

  printf("Oберіть файл який потрібно взламати\n\n");
  filename_read = OpenFileDialog();
  if (!filename_read) return;
  readFromFile(filename_read, encryptedText);

  printf("Oберіть файл для запису знайдених ключів\n\n");
  filename_write = OpenFileDialog();
  if (!filename_write) return;

  printf("Насолоджуйтесь підбором ключів!\n");

  FILE* file = fopen(filename_write, "w");
  fclose(file);
  file = fopen(filename_write, "a");

  if (file == NULL) {
    printf("Помилка відкриття файлу для запису!\n");
    return;
  }

  for (a = 2; a < alphabet.size(); a++) {
    if (mod_inverse(a) == -1 || !is_simple(a)) continue;  // Пропускаємо, якщо немає обернення

    for (b = 0; b < alphabet.size(); b++) {
      char decryptedText[1024];
      affine_decrypt(encryptedText, decryptedText, a, b);
      fprintf(file, "Спроба a = %d, b = %d: %s\n", a, b, decryptedText);

      for (i = 0; i < sizeof(commonWords) / sizeof(commonWords[0]); i++) {
        
        j = 0;
        strcpy(text_toLower, decryptedText);
        for (j = 0; text_toLower[j] != '\0'; j++) {
          text_toLower[j] = tolower(text_toLower[j]);
        }
        if (strstr(text_toLower, commonWords[i]) != NULL) {
          printf("Спроба a = %d, b = %d: (%s) %s\n", a, b, commonWords[i], decryptedText);
          break;
        }
      }

    }
  }
  fclose(file);
  printf(RED"*ПРИМІТКА* " RESET);
  printf("Повний набір ключів наведений у файлі:\n%s\n\n", filename_write);
};


int main() {
  system("chcp 1251 & cls");

  int user_choice = -1;

  while (user_choice != 0) {
    printf("====================--Проєкт \"Афінне шифрування\"--====================\n");
    printf("Чим бажаєте зайнятись?\n");
    printf("0 - Вийти з консолі\n");
    printf("1 - Зашифрувати повідомлення    (з консолі)\n");
    printf("2 - Розшифрувати повідомлення   (з консолі)\n");
    printf("3 - Зашифрувати повідомлення    (з файлу)\n");
    printf("4 - Розшифрувати повідомлення   (з файлу)\n");
    printf("5 - Взламати шифр               (з файлу)\n");
    printf("То як, ваш вибір: ");

    do {
      scanf_s("%d", &user_choice);
      if (user_choice < 0 || user_choice > 5) {
        printf("\aТакої опції не існує!\nОберіть від 0 - 5: ");
        while (char c = getchar() != '\n');
      }

    } while (user_choice < 0 || user_choice > 5);

    while (getchar() != '\n') { ; };

    switch (user_choice) {
    case 0: { user_choice = 0; break; };
    case 1: { console_encrypting(); break; };
    case 2: { console_decrypting(); break; };
    case 3: { file_encrypting();  break; };
    case 4: { file_decrypting();  break; };
    case 5: { bruteForce();  break; };
    default: {};
    }

    system("pause");
    system("cls");
  }
  return 0;
}
