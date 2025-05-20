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

std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.,!?;:()[]{}\"'\\|/�@#$%&*_-+= ";    // ������� ������� (������� � ����������� ���������, � ����� \ \ ����� �������)

// ������ �������� ���
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

// ������� ��� ����������� ��������������� ����������
int mod_inverse(int a) {
  int i;
  for (i = 1; i < alphabet.size(); i++) {
    if ((a * i) % alphabet.size() == 1)
      return i;
  }
  return -1; // ���� �������� ����� �� ��������
}

// ������� �������� �� ����� � �������
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

// ������� ����������
void affine_encrypt(const char* plaintext, char* ciphertext, int a, int b) {
  int i, x, y;

  for (i = 0; i < strlen(plaintext); i++) {
    const char* found = strchr(alphabet.c_str(), plaintext[i]);   // strchr - ���� ������ � ������, ��������� ������� - ������ ����� �� ����������� ������ ����� �����
    if (found) {
      x = found - alphabet.c_str();       // ������� ������� � ������
      y = (a * x + b) % alphabet.size();
      ciphertext[i] = alphabet[y]; // ������������ ������
    }
    else {
      ciphertext[i] = plaintext[i]; // �������� ������ ��� ���
    }
  }
  ciphertext[strlen(plaintext)] = '\0'; // ������ ���������� �����
}

// ������� ������������
void affine_decrypt(const char* ciphertext, char* plaintext, int a, int b) {
  int n = alphabet.size(),
    a_inverse = mod_inverse(a),
    y,        // ������ ������� � ������ (��� ������������� �����������)
    x,        // ������ ������� � ������ (��� �������������� �����������)
    i;        // ������ ������� � ������� ������

  for (i = 0; i < strlen(ciphertext); i++) {
    y = alphabet.find(ciphertext[i]); // ��������� ������� ������� � ������
    if (y != std::string::npos) { // ���� ������ ��������
      x = (a_inverse * ((y - b + n) % n)) % n; // �������� ���������� x
      x = (x + n) % n;      // ��� ����� ������� �� a = 3, b = 2
      plaintext[i] = alphabet[x]; // ������������
    }
    else {
      plaintext[i] = ciphertext[i]; // ���� ������ �� ��������, �������� ��� ���
    }
  }

  plaintext[strlen(ciphertext)] = '\0';
}

// ������� ������ � ����
void writeToFile(const char* filename, const char* message, const char* mode) {
  FILE* file = fopen(filename, mode);
  if (file == NULL) {
    printf("������� �������� ����� ��� ������!\n");
    return;
  }

  fprintf(file, "%d %s", strlen(message), message);
  fclose(file);
}

// ������� ������� �����
void readFromFile(const char* filename, char* message) {
  int size = 0;

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    printf("������� �������� ����� ��� ����������!\n");
    return;
  }

  fscanf(file, "%d ", &size);
  fgets(message, size + 1, file);
  fclose(file);
}

// ������� ������ �����
char* OpenFileDialog() {
  static wchar_t szFile[MAX_PATH] = { 0 }; // ����� ��� �������� �����
  static char filePath[MAX_PATH] = { 0 }; // ����� ��� �������������� ����� � char
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
    printf("������� ����: %s\n", filePath);
    return filePath;
  }
  else {
    printf("���� ����� ��������� ������������.\n");
    return NULL;
  }
}

// ������� ������ �� ���������� �����
void askToSaveToFile(const char* message) {
  int save_inFile;

  printf("\n�������� � ���� (0 / 1): ");
  scanf("%d", &save_inFile);
  if (save_inFile == 1) {
    printf("\n������ ����, ���� ��������\n");
    const char* filename = OpenFileDialog();
    if (!filename) return;
    writeToFile(filename, message, "w");
  }
}

// ������� ������ ������������ ��������� ����������
void inputCryptingParam(int* a, int* b) {
  do {
    printf("������ ������� ������� (0 < a < %d): ", alphabet.size() + 1);
    scanf("%d", a);
    if (*a <= 0 || *a > alphabet.size())
      printf("����� �� ������\n");
    else if (!is_simple(*a))
      printf("����� �� ������, �� ���� �������\n");
    else { ; }
  } while (!is_simple(*a) || (*a <= 0 || *a > alphabet.size()));

  printf("������ ���� (0 < b < %d): ", alphabet.size() + 1);
  scanf("%d", b);
}

// ������� ���������� ��������
void console_encrypting() {
  int a, b, n;
  char plaintext[1024];         // ����� ��� ���������� ��������� ������ �� ����������
  char ciphertext[1024];       // ����� ��� ���������� ������������� ������

  printf("������ �������: %d\n", alphabet.size());
  printf("������ ����� ��� ����������: ");
  fgets(plaintext, sizeof(plaintext), stdin);

  plaintext[strcspn(plaintext, "\n")] = '\0'; // ��������� ������� ������ �����


  inputCryptingParam(&a, &b);

  affine_encrypt(plaintext, ciphertext, a, b);
  printf("������������ �����: %s%s%s\n", GREEN, ciphertext, RESET);

  askToSaveToFile(ciphertext);
};

// ������� ������������ ��������
void console_decrypting() {
  int a, b, n;
  char ciphertext[1024];       // ����� ��� ���������� ������������� ������
  char decryptedtext[1024];   // ����� ��� ���������� �������������� ������

  printf("������ �������: %d\n", alphabet.size());
  printf("������ ����� ��� ����������: ");
  fgets(ciphertext, sizeof(ciphertext), stdin);

  ciphertext[strcspn(ciphertext, "\n")] = '\0'; // ��������� ������� ������ �����

  inputCryptingParam(&a, &b);

  affine_decrypt(ciphertext, decryptedtext, a, b);
  printf("������������� �����: %s%s%s\n", GREEN, decryptedtext, RESET);

  askToSaveToFile(decryptedtext);
};

// ������� ���������� �����
void file_encrypting() {
  int a, b, save_inFile;
  char ciphertext[1024],       // ����� ��� ���������� ������������� ������
    plaintext[1024];          // ��������� �����

  const char* filename_read;


  filename_read = OpenFileDialog();
  if (!filename_read) return;
  readFromFile(filename_read, plaintext);

  inputCryptingParam(&a, &b);

  affine_encrypt(plaintext, ciphertext, a, b);
  printf("������������ �����: %s%s%s\n", GREEN, ciphertext, RESET);

  askToSaveToFile(ciphertext);
};

// ������� ������������ �����
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
  printf("������������� �����: %s%s%s\n", GREEN, plaintext, RESET);

  askToSaveToFile(plaintext);
};

// ������� Brute Force
void bruteForce() {
  const char* filename_read;
  const char* filename_write;
  char encryptedText[1024],
    text_toLower[1024];

  int a, b, i, j;

  printf("O����� ���� ���� ������� ��������\n\n");
  filename_read = OpenFileDialog();
  if (!filename_read) return;
  readFromFile(filename_read, encryptedText);

  printf("O����� ���� ��� ������ ��������� ������\n\n");
  filename_write = OpenFileDialog();
  if (!filename_write) return;

  printf("�������������� ������� ������!\n");

  FILE* file = fopen(filename_write, "w");
  fclose(file);
  file = fopen(filename_write, "a");

  if (file == NULL) {
    printf("������� �������� ����� ��� ������!\n");
    return;
  }

  for (a = 2; a < alphabet.size(); a++) {
    if (mod_inverse(a) == -1 || !is_simple(a)) continue;  // ����������, ���� ���� ���������

    for (b = 0; b < alphabet.size(); b++) {
      char decryptedText[1024];
      affine_decrypt(encryptedText, decryptedText, a, b);
      fprintf(file, "������ a = %d, b = %d: %s\n", a, b, decryptedText);

      for (i = 0; i < sizeof(commonWords) / sizeof(commonWords[0]); i++) {
        
        j = 0;
        strcpy(text_toLower, decryptedText);
        for (j = 0; text_toLower[j] != '\0'; j++) {
          text_toLower[j] = tolower(text_toLower[j]);
        }
        if (strstr(text_toLower, commonWords[i]) != NULL) {
          printf("������ a = %d, b = %d: (%s) %s\n", a, b, commonWords[i], decryptedText);
          break;
        }
      }

    }
  }
  fclose(file);
  printf(RED"*���̲���* " RESET);
  printf("������ ���� ������ ��������� � ����:\n%s\n\n", filename_write);
};


int main() {
  system("chcp 1251 & cls");

  int user_choice = -1;

  while (user_choice != 0) {
    printf("====================--����� \"������ ����������\"--====================\n");
    printf("��� ������ ���������?\n");
    printf("0 - ����� � ������\n");
    printf("1 - ����������� �����������    (� ������)\n");
    printf("2 - ������������ �����������   (� ������)\n");
    printf("3 - ����������� �����������    (� �����)\n");
    printf("4 - ������������ �����������   (� �����)\n");
    printf("5 - �������� ����               (� �����)\n");
    printf("�� ��, ��� ����: ");

    do {
      scanf_s("%d", &user_choice);
      if (user_choice < 0 || user_choice > 5) {
        printf("\a���� ����� �� ����!\n������ �� 0 - 5: ");
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
