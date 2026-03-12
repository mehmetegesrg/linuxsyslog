# Linux Syslog Okuyucu ve Bağlı Liste Uygulaması

Bu proje, Linux işletim sistemlerindeki sistem günlüklerini (`"syslog.txt"`) okuyarak bu verileri bellekte yapılandırılmış bir şekilde tutmak amacıyla geliştirilmiştir. Geleneksel dosya okuma yöntemleri yerine, log yönetimi ve veri işleme performansını artırmak için **Çift Yönlü Bağlı Liste (Doubly Linked List)** veri yapısı kullanılmıştır.

**Geliştirici:** Mehmet Ege Sarıgöl  
**Kurum:** Kırklareli Üniversitesi - Yazılım Mühendisliği Bölümü  

## 📌 Projenin Amacı

İşletim sistemleri, çekirdek (kernel) ve servis düzeyindeki tüm olayları (hata, uyarı, yetkilendirme işlemleri) syslog mekanizması üzerinden kaydeder. Sistem yöneticileri bir problemi analiz ederken genellikle dosyanın en başındaki (en eski) kayıtlardan ziyade, **en son eklenen (en yeni)** kayıtlara odaklanırlar.

Bu projede Syslog mekanizmasının dinamik yapısına en uygun veri yapısının tasarlanması ve bu sayede sistem kayıtlarının simüle edilmesi amaçlanmıştır.

## 🛠 Kullanılan Veri Yapısı: Çift Yönlü Bağlı Liste (Doubly Linked List)

Bu projede tek yönlü (singly) bağlı liste veya dizi (array) yerine **Çift Yönlü Bağlı Liste** tercih edilmiştir. Bunun temel sebepleri:

1. **Geriye Dönük Okuma Esnekliği:** Çift yönlü bağlı listede her düğüm hem bir önceki (`prev`) hem de bir sonraki (`next`) düğümün bellek adresini tutar. Loglar okunurken kuyruğa (`tail`) O(1) karmaşıklığı ile eklenir. Okunmak istendiğinde ise kuyruktan (`tail`) başa (`head`) doğru geriye gidilerek en güncel loglar anında ekrana basılabilir.
2. **Dinamik Bellek Yönetimi:** Log dosyalarının boyutu önceden bilinemez ve sürekli büyür. Sabit boyutlu diziler (array) bellek israfına veya taşmalara (overflow) yol açarken, bağlı listeler sadece ihtiyaç duyulduğu an (runtime sırasında) `malloc()` ile bellek tahsis ederek sistemi yormaz.

## 🚀 Kurulum ve Çalıştırma

Proje **C programlama dili** ile geliştirilmiş olup, herhangi bir harici kütüphaneye ihtiyaç duymadan doğrudan GCC derleyicisi ile derlenebilir.

### 1. Repoyu Klonlayın
```bash
git clone [https://github.com/KULLANICI_ADIN/syslog-linked-list.git](https://github.com/KULLANICI_ADIN/syslog-linked-list.git)
cd syslog-linked-list

```
2. Kodu Derleyin
```bash
gcc main.c -o syslog_reader
```

3. Uygulamayı Çalıştırın
Linux sistemlerinde /var/log/syslog dosyasını okumak sistem yetkisi (root privileges) gerektirebilir. Bu nedenle uygulamayı sudo ile çalıştırmanız tavsiye edilir:

```bash
sudo ./syslog_reader

```
📋 Çıktı Örneği
Uygulama çalıştığında, logları belleğe alır ve analiz kolaylığı sağlamak için en son üretilen logları ilk sırada gösterecek şekilde (Reverse Order) ekrana yazdırır.

```bash
Plaintext
Syslog dosyasindaki veriler basariyla Cift Yonlu Bagli Listeye aktarildi.

--- SYSLOG KAYITLARI (EN YENIDEN EN ESKIYE) ---
Mar 12 10:45:01 ege-pc CRON[1234]: (root) CMD (command -v debian-sa1 > /dev/null ...)
Mar 12 10:42:15 ege-pc systemd[1]: Started Session 4 of user ege.
Mar 12 10:41:03 ege-pc kernel: [ 10.123456] wlp2s0: authenticated
...
-----------------------------------------------
```
