#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bir log satırının maksimum uzunluğu
#define MAX_LINE_LENGTH 1024

/* * Çift Yönlü Bağlı Liste (Doubly Linked List) Düğüm Yapısı
 * Her bir düğüm bir Syslog satırını temsil eder.
 */
typedef struct SyslogNode {
    char *logMessage;           // Syslog mesajını tutacak dinamik karakter dizisi
    struct SyslogNode *prev;    // Bir önceki log kaydını gösteren işaretçi
    struct SyslogNode *next;    // Bir sonraki log kaydını gösteren işaretçi
} SyslogNode;

// Bağlı listenin başını ve sonunu tutacak global işaretçiler
SyslogNode *head = NULL;
SyslogNode *tail = NULL;

/*
 * Yeni bir log düğümü oluşturan fonksiyon.
 * Parametre olarak okunan log satırını alır.
 */
SyslogNode* createLogNode(const char *message) {
    // Bellekte yeni düğüm için yer ayırıyoruz
    SyslogNode *newNode = (SyslogNode*)malloc(sizeof(SyslogNode));
    if (newNode == NULL) {
        printf("Bellek tahsis hatasi!\n");
        exit(1);
    }

    // Gelen mesaj için bellekte yer ayırıp kopyalıyoruz
    newNode->logMessage = (char*)malloc(strlen(message) + 1);
    if (newNode->logMessage == NULL) {
        printf("Mesaj icin bellek tahsis hatasi!\n");
        exit(1);
    }
    strcpy(newNode->logMessage, message);

    // İşaretçileri başlangıçta NULL yapıyoruz
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

/*
 * Okunan log kaydını çift yönlü bağlı listenin sonuna (kuyruğa) ekleyen fonksiyon.
 * Zaman karmaşıklığı: O(1) - Çünkü tail (kuyruk) işaretçisini biliyoruz.
 */
void appendLogEntry(const char *message) {
    SyslogNode *newNode = createLogNode(message);

    // Eğer liste boşsa, ilk düğüm head ve tail olur
    if (head == NULL) {
        head = newNode;
        tail = newNode;
    } else {
        // Liste boş değilse, yeni düğümü tail'in sonuna ekliyoruz
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode; // Tail artık yeni düğümümüz oldu
    }
}

/*
 * Logları sondan başa doğru (en yeninden en eskiye) okuyan fonksiyon.
 * Çift Yönlü Bağlı Liste kullanmamızın temel sebebi bu fonksiyondur!
 */
void displayLogsReverse() {
    SyslogNode *current = tail;
    int count = 0;

    printf("\n--- SYSLOG KAYITLARI (EN YENIDEN EN ESKIYE) ---\n");
    // tail'den başlayıp prev işaretçilerini takip ederek head'e kadar gidiyoruz
    while (current != NULL) {
        printf("%s", current->logMessage);
        current = current->prev;
        count++;
        // Çok fazla log olabileceği için sadece son 20 logu gösteriyoruz (Demo amaçlı)
        if (count >= 20) break;
    }
    printf("-----------------------------------------------\n");
}

/*
 * Program sonlanırken bellekte (Heap) ayırdığımız alanları temizleyen fonksiyon.
 * Memory leak (bellek sızıntısı) oluşmasını engeller.
 */
void freeLogList() {
    SyslogNode *current = head;
    SyslogNode *temp;

    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp->logMessage); // Önce string için ayrılan belleği serbest bırak
        free(temp);             // Sonra düğümün kendisini serbest bırak
    }
}

int main() {
    // Linux sistemlerinde genel syslog dosyasının yolu
    const char *logFilePath = "syslog.txt";
    FILE *file = fopen(logFilePath, "r");

    // Dosyanın açılıp açılamadığını kontrol ediyoruz
    if (file == NULL) {
        printf("Hata: %s dosyasi acilamadi.\n", logFilePath);
        printf("Lutfen programi 'sudo' ile calistirdiginizdan veya dosya yolunun dogru oldugundan emin olun.\n");
        return 1;
    }

    char buffer[MAX_LINE_LENGTH];

    // Dosyayı satır satır okuyup bağlı listemize ekliyoruz
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        appendLogEntry(buffer);
    }

    fclose(file);
    printf("Syslog dosyasindaki veriler basariyla Cift Yonlu Bagli Listeye aktarildi.\n");

    // Logları sistem yöneticisi mantığıyla (sondan başa doğru) ekrana yazdırıyoruz
    displayLogsReverse();

    // Belleği temizliyoruz
    freeLogList();

    return 0;
}