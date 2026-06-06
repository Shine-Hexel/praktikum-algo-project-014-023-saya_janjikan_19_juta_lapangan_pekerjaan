#include <iostream>
#include <iomanip>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
using namespace std;

struct tugas
{
    char namaTugas[100];
    char deskripsi[200];
    char status[50];
    int tanggal;
    int bulan;
    int tahun;
    int jam;
    int menit;

    tugas *next;
    tugas *prev;
};

tugas *head = nullptr;
tugas *tail = nullptr;

void tampilMenu();
void tandaiTugasSelesai();
void pindahTugasSelesai(tugas *node);
void lihatTugasSelesai();
void aturStatusTerkini(tugas *node);
void simpanDataTugas();
void ngambilDataTugas();
void lihatTugas();
void nambahTugasBaru();
void sortingTugasDL();
void sortingTugasJudul();
void searchTask();
void hapusTask();
void bersihkanList();

int main()
{

    cout << endl;

    char pilih;
    bool running = true;

    while (running)
    {
        tampilMenu();
        cin >> pilih;

        switch (toupper(pilih))
        {
        case 'A':
            nambahTugasBaru();
            break;
        case 'B':
            hapusTask();
            break;
        case 'C':
            if (head == nullptr)
            {
                cout << "Belum ada tugas untuk direfresh." << endl;
                break;
            }
            simpanDataTugas();
            cout << "Status berhasil direfresh" << endl;
            break;
        case 'D':
            searchTask();
            break;
        case 'E':
            if (head == nullptr)
            {
                cout << "Belum ada tugas untuk disorting." << endl;
                break;
            }
            char pilihSort;
            cout << "\nPilih metode sorting:\n";
            cout << "1. Sorting by Deadline (DL)\n";
            cout << "2. Sorting by Nama Tugas (A-Z)\n";

        pilihsortt:
        {

            cout << "Pilihan: ";
            cin >> pilihSort;
            if (pilihSort == '1')
            {
                sortingTugasDL();
                cout << "\nTugas berhasil disorting berdasarkan deadline!\n\n";
            }
            else if (pilihSort == '2')
            {
                sortingTugasJudul();
                cout << "\nTugas berhasil disorting berdasarkan judul tugas!\n\n";
            }
            else
            {
                cout << "\nPilihan sorting tidak valid!\nSilahkan pilih antara opsi 1 atau 2.\n\n";
                goto pilihsortt;
            }
        }
        break;
        case 'F':
            lihatTugasSelesai();
            break;
        case 'H':
            tandaiTugasSelesai();
            break;
        case 'G':
            cout << "\n  Selamat fokus menyelesaikan tugas! Data tersimpan secara otomatis >_< \n\n";
            return 0;
        default:
            cout << "\nPilihan tidak valid!\nSilahkan kembali ke menu lalu pilih antara opsi A-G.\n";
        }

    lanjuut:
    {

        string lanjut_gak_nich;
        cout << "Kembali ke menu? (y/t): ";
        cin >> lanjut_gak_nich;

        if (lanjut_gak_nich == "t" || lanjut_gak_nich == "T")
        {
            cout << "\n  Selamat fokus menyelesaikan tugas! Data tersimpan secara otomatis >_< \n\n";
            running = false;
            break;
        }

        else if (lanjut_gak_nich == "y" || lanjut_gak_nich == "")
        {
            system("cls");
        }

        else
        {
            cout << "Input tidak valid" << endl;
            goto lanjuut;
        }
    }
    }
}

void tampilMenu()
{
    lihatTugas();
    cout << "========================================\n";
    cout << "|              MENU UTAMA              |\n";
    cout << "========================================\n";
    cout << "    [A] Tambah Tugas Baru\n";
    cout << "    [B] Hapus Tugas\n";
    cout << "    [C] Refresh Tugas\n";
    cout << "    [D] Cari Tugas\n";
    cout << "    [E] Sorting Tugas\n";
    cout << "    [F] Lihat Tugas Done\n";
    cout << "    [H] Tandai Tugas Selesai\n";
    cout << "    [G] Logout\n";
    cout << "========================================\n";
    cout << "  Pilihan: ";
}

void pindahTugasSelesai(tugas *node)
{
    FILE *file = fopen("data_tugas_done.txt", "a");

    if (file == nullptr)
    {
        cout << "File gagal dibuka!\n";
        return;
    }

    fprintf(file, "%s;%s;%s;%d;%d;%d;%d;%d\n",
            node->namaTugas,
            node->deskripsi,
            node->status,
            node->tanggal,
            node->bulan,
            node->tahun,
            node->jam,
            node->menit);

    if (node == head) // jika node yang dipindahkan adalah head, head diupdate ke node berikutnya
    {
        head = node->next;
        if (head != nullptr) // jika list tidak menjadi kosong setelah head diupdate, prev head yang baru diupdate jadi nullptr
            head->prev = nullptr;
    }
    else if (node == tail) // jika node yang dipindahkan adalah tail, tail diupdate ke node sebelumnya
    {
        tail = node->prev;
        if (tail != nullptr) // jika list tidak menjadi kosong setelah tail diupdate, next tail yang baru diupdate jadi nullptr
            tail->next = nullptr;
    }
    else // jika node berada di tengah, next node sebelumnya diupdate ke node berikutnya, dan prev node berikutnya diupdate ke node sebelumnya
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->next = nullptr;
        node->prev = nullptr;
    }

    delete node;
    fclose(file);

    simpanDataTugas(); // update file data_tugas.txt setelah memindahkan tugas ke data_tugas_done.txt
}

void tandaiTugasSelesai()
{
    if (head == nullptr)
    {
        cout << "Belum ada tugas.\n";
        return;
    }
    char namaTugas[100];
    cout << "Masukkan nama tugas yang ingin ditandai selesai: ";
    cin.ignore();
    cin.getline(namaTugas, 100);

    tugas *bantu = head;
    bool found = false;

    while (bantu != nullptr)
    {
        if (strcmp(bantu->namaTugas, namaTugas) == 0)
        {
            strcpy(bantu->status, "Done");
            pindahTugasSelesai(bantu); // pindahkan tugas yang sudah selesai ke file data_tugas_done.txt dan hapus dari list tugas
            cout << "\nTugas \"" << namaTugas << "\" berhasil ditandai selesai!\n\n";
            found = true;
            break;
        }
        bantu = bantu->next;
    }

    if (!found) // jika tugas dengan nama yang dimasukkan tidak ditemukan dalam list tugas
    {
        cout << endl;
        cout << "Tugas \"" << namaTugas << "\" tidak ditemukan." << endl
             << endl;
    }
}

void aturStatusTerkini(tugas *node) // nanti dipake buat refresh status tugas juga
{
    time_t saat_ini = time(0); // ambil waktu saat ini dalam format time_t

    tm deadline = {}; // inisialisasi struct tm untuk deadline tugas

    deadline.tm_year = node->tahun - 1900; // tm_year dihitung sejak 1900
    deadline.tm_mon = node->bulan - 1;     // tm_mon dihitung dari 0 (Januari)
    deadline.tm_mday = node->tanggal;
    deadline.tm_hour = node->jam;
    deadline.tm_min = node->menit;
    // deadline.tm_sec = 0;

    time_t waktuDeadline = mktime(&deadline); // konversi struct tm ke time_t untuk perbandingan

    if (waktuDeadline < saat_ini)
    {
        strcpy(node->status, "Late");
    }
    else
    {
        strcpy(node->status, "In Progress");
    }
}

void hapusTask()
{

    if (head == nullptr)
    {
        cout << "Tidak ada tugas untuk dihapus.\n";
        return;
    }

    char keyword[100];

    cout << "[PASTIKAN PENULISAN NAMA TUGAS SAMA DENGAN PENULISAN PADA SAAT INPUT]\n";
    cout << "Masukkan nama tugas yang ingin dihapus: ";

    cin.ignore();
    cin.getline(keyword, 100);

    tugas *bantu = head;
    bool found = false;

    while (bantu != nullptr)
    {
        if (strcmp(bantu->namaTugas, keyword) == 0) // jika nama tugas sesuai dengan keyword yang dicari
        {
            // jika node yang dihapus adalah head
            if (bantu == head)
            {
                head = bantu->next;

                if (head != nullptr)
                {
                    head->prev = nullptr;
                }
                else
                {
                    // jika list menjadi kosong
                    tail = nullptr;
                }
            }

            // jika node yang dihapus adalah tail
            else if (bantu == tail)
            {
                tail = bantu->prev;

                if (tail != nullptr)
                {
                    tail->next = nullptr;
                }
            }

            // jika node berada di tengah
            else
            {
                bantu->prev->next = bantu->next;
                bantu->next->prev = bantu->prev;
            }

            delete bantu;

            // simpan ulang file
            simpanDataTugas();

            found = true;

            cout << "\nTugas \"" << keyword << "\" berhasil dihapus!\n\n";

            break;
        }

        bantu = bantu->next;
    }

    if (!found)
    {
        cout << "\nTugas \"" << keyword << "\" tidak ditemukan.\n\n";
    }
}

void simpanDataTugas()
{
    // buat nyimpen data tugas ke file
    FILE *file = fopen("data_tugas.txt", "w");
    tugas *bantu = head;

    if (file == nullptr)
    {
        cout << "Gagal membuka file data_tugas.txt!" << endl;
        return;
    }

    while (bantu != nullptr)
    {
        fprintf(file, "%s;%s;%s;%d;%d;%d;%d;%d\n",
                bantu->namaTugas,
                bantu->deskripsi,
                bantu->status,
                bantu->tanggal,
                bantu->bulan,
                bantu->tahun,
                bantu->jam,
                bantu->menit);

        bantu = bantu->next;
    }
    fclose(file);
}

void bersihkanList()
// buat bersihin list tugas sebelum ngambil data dari file, biar gak numpuk data yang sama
{
    tugas *bantu = head;
    while (bantu != nullptr)
    {
        tugas *temp = bantu;
        bantu = bantu->next;
        delete temp;
    }
    head = tail = nullptr;
}

void ngambilDataTugas()
{
    // bersihin dulu list tugasnya
    bersihkanList();
    // ngambil data tugas dari file
    FILE *file = fopen("data_tugas.txt", "r");
    if (file == nullptr)
    {
        cout << "Tugas masih kosong" << endl;
        cout << "----------------------------------------" << endl;
        cout << endl;
        return;
    }
    else if (head == nullptr)
    {
        cout << "Belum ada tugas yang tersimpan" << endl;
        cout << "----------------------------------------" << endl;
        cout << endl;
        return;
    }

    tugas *bantu = new tugas;

    while (fscanf(file, "%[^;];%[^;];%[^;];%d;%d;%d;%d;%d\n",
                  bantu->namaTugas,
                  bantu->deskripsi,
                  bantu->status,
                  &bantu->tanggal,
                  &bantu->bulan,
                  &bantu->tahun,
                  &bantu->jam,
                  &bantu->menit) != EOF)
    {
        tugas *baru = new tugas;

        // masukin data dari node bantu ke node tugas
        strcpy(baru->namaTugas, bantu->namaTugas);
        strcpy(baru->deskripsi, bantu->deskripsi);
        strcpy(baru->status, bantu->status);
        baru->tanggal = bantu->tanggal;
        baru->bulan = bantu->bulan;
        baru->tahun = bantu->tahun;
        baru->jam = bantu->jam;
        baru->menit = bantu->menit;

        baru->next = nullptr;
        baru->prev = nullptr;

        // status tugas diupdate biar tetep sinkron dan sesuai dengan waktu sekarang
        // kalo udah lewat deadline, statusnya jadi "Late", kalo belum lewat, statusnya "In Progress"
        aturStatusTerkini(baru);

        // jika list masih kosong, node baru jadi head dan tail
        if (head == nullptr)
        {
            head = tail = baru;
        }

        // kalo list udah ada isinya, node baru ditambahin di akhir list
        else
        {
            tail->next = baru;
            baru->prev = tail;
            tail = baru;
        }
    }

    fclose(file);
}

void lihatTugas()
{
    cout << "+========================================+\n";
    cout << "|              TUGAS SAYA               |\n";
    cout << "+========================================+\n";

    ngambilDataTugas();
    tugas *bantu = head;

	while (bantu != nullptr) {
        cout << "| " << left << setw(12) << "Nama Tugas" << ": " << bantu->namaTugas << "\n";
        cout << "| " << left << setw(12) << "Deskripsi"  << ": " << bantu->deskripsi << "\n";
        cout << "| " << left << setw(12) << "Status"     << ": " << bantu->status << "\n";
        cout << "| " << left << setw(12) << "Deadline"   << ": "
             << setfill('0') << setw(2) << bantu->tanggal << "/"
             << setw(2) << bantu->bulan << "/"
             << setfill(' ') << bantu->tahun
             << "  " << setfill('0') << setw(2) << bantu->jam << ":"
             << setw(2) << bantu->menit << setfill(' ') << "\n";
        cout << "+----------------------------------------+\n";
        bantu = bantu->next;
    }
    delete bantu;
}

void lihatTugasSelesai()
{
    // buat nampilin tugas yang udah selesai dari file data_tugas_done.txt
    FILE *file = fopen("data_tugas_done.txt", "r");
    if (file == nullptr)
    {
        cout << "Tugas masih kosong" << endl;
        cout << endl;
        return;
    }

    cout << endl;
    cout << "========================================\n";
    cout << "|            TUGAS SELESAI             |\n";
    cout << "========================================\n";

    char namaTugas[100];
    char deskripsi[200];
    char status[50];
    int tanggal, bulan, tahun, jam, menit;

    while (fscanf(file, "%[^;];%[^;];%[^;];%d;%d;%d;%d;%d\n",
              namaTugas,
              deskripsi,
              status,
              &tanggal,
              &bulan,
              &tahun, 
              &jam, 
              &menit) != EOF) {
    cout << "| " << left << setw(12) << "Nama Tugas" << ": " << namaTugas << "\n";
    cout << "| " << left << setw(12) << "Deskripsi"  << ": " << deskripsi << "\n";
    cout << "| " << left << setw(12) << "Status"     << ": " << status << "\n";
    cout << "| " << left << setw(12) << "Deadline"   << ": "
         << setfill('0') << setw(2) << tanggal << "/"
         << setw(2) << bulan << "/"
         << setfill(' ') << tahun
         << "  " << setfill('0') << setw(2) << jam << ":"
         << setw(2) << menit << setfill(' ') << "\n";
    cout << "+----------------------------------------+\n";
	}

    fclose(file);
}

void nambahTugasBaru()
{
    // buat nambah tugas baru
    tugas *baru = new tugas;

    cout << "Masukkan nama tugas: ";
    cin.ignore();
    cin.getline(baru->namaTugas, 100);

    cout << "Masukkan deskripsi tugas: ";
    cin.getline(baru->deskripsi, 200);

    do
    {
        cout << "Masukkan tanggal deadline (1-31): ";
        cin >> baru->tanggal;
    } while (baru->tanggal < 1 || baru->tanggal > 31); // validasi input tanggal antara 1-31

    do
    {
        cout << "Masukkan bulan deadline (1-12): ";
        cin >> baru->bulan;
    } while (baru->bulan < 1 || baru->bulan > 12); // validasi input bulan antara 1-12

    cout << "Masukkan tahun deadline: ";
    cin >> baru->tahun;

    do
    {
        cout << "Masukkan jam deadline (0-23): "; // validasi input jam antara 0-23
        cin >> baru->jam;
    } while (baru->jam < 0 || baru->jam > 23);

    do
    {
        cout << "Masukkan menit deadline (0-59): "; // validasi input menit antara 0-59
        cin >> baru->menit;
    } while (baru->menit < 0 || baru->menit > 59);

    // status tugas diupdate sesuai dengan waktu sekarang
    // kalo udah lewat deadline, statusnya jadi "Late", kalo belum lewat maka "in progress"
    aturStatusTerkini(baru);

    baru->next = nullptr;
    baru->prev = nullptr;

    if (head == nullptr) // jika list masih kosong, node baru jadi head dan tail
    {
        head = tail = baru;
    }
    else // kalo list udah ada isinya, node baru ditambahin di akhir list
    {
        tail->next = baru;
        baru->prev = tail;
        tail = baru;
    }

    simpanDataTugas();
    cout << "Tugas baru berhasil ditambahkan!\n\n";
}

void sortingTugasDL()
{

    tugas *bantu = head;

    while (bantu != nullptr)
    {
        // bikin pointer min untuk nyimpen tugas dengan deadline paling awal,
        // mulai dari tugas yang sedang diperiksa (bantu) sampai akhir list
        tugas *min = bantu;
        tugas *selanjutnya = bantu->next;

        while (selanjutnya != nullptr)
        {
            if ( // membandingkan deadline tugas
                (selanjutnya->tahun < min->tahun) ||
                (selanjutnya->tahun == min->tahun && selanjutnya->bulan < min->bulan) ||
                (selanjutnya->tahun == min->tahun && selanjutnya->bulan == min->bulan && selanjutnya->tanggal < min->tanggal) ||
                (selanjutnya->tahun == min->tahun && selanjutnya->bulan == min->bulan && selanjutnya->tanggal == min->tanggal && selanjutnya->jam < min->jam) ||
                (selanjutnya->tahun == min->tahun && selanjutnya->bulan == min->bulan && selanjutnya->tanggal == min->tanggal && selanjutnya->jam == min->jam && selanjutnya->menit < min->menit))
            {
                min = selanjutnya;
            }
            selanjutnya = selanjutnya->next;
        }

        if (min != bantu)
        {
            // swap isi data tugas menggunakan variabel sementara
            char tempNama[100], tempDeskripsi[200], tempStatus[50];
            int tempTanggal, tempBulan, tempTahun, tempJam, tempMenit;

            strcpy(tempNama, bantu->namaTugas);
            strcpy(tempDeskripsi, bantu->deskripsi);
            strcpy(tempStatus, bantu->status);
            tempTanggal = bantu->tanggal;
            tempBulan = bantu->bulan;
            tempTahun = bantu->tahun;
            tempJam = bantu->jam;
            tempMenit = bantu->menit;

            strcpy(bantu->namaTugas, min->namaTugas);
            strcpy(bantu->deskripsi, min->deskripsi);
            strcpy(bantu->status, min->status);
            bantu->tanggal = min->tanggal;
            bantu->bulan = min->bulan;
            bantu->tahun = min->tahun;
            bantu->jam = min->jam;
            bantu->menit = min->menit;

            strcpy(min->namaTugas, tempNama);
            strcpy(min->deskripsi, tempDeskripsi);
            strcpy(min->status, tempStatus);
            min->tanggal = tempTanggal;
            min->bulan = tempBulan;
            min->tahun = tempTahun;
            min->jam = tempJam;
            min->menit = tempMenit;
        }

        bantu = bantu->next;
    }

    simpanDataTugas(); // update file data_tugas.txt setelah sorting berdasarkan deadline
}

// sorting by judul tugas secara alfabetis menggunakan bubble sort
void sortingTugasJudul()
{

    bool swapped;
    tugas *bantu;
    tugas *akhir = nullptr;

    do
    {
        swapped = false;
        bantu = head;

        while (bantu->next != akhir)
        {
            if (strcmp(bantu->namaTugas, bantu->next->namaTugas) > 0)
            {
                // swap isi data tugas menggunakan variabel sementara
                char tempNama[100], tempDeskripsi[200], tempStatus[50];
                int tempTanggal, tempBulan, tempTahun, tempJam, tempMenit;

                strcpy(tempNama, bantu->namaTugas);
                strcpy(tempDeskripsi, bantu->deskripsi);
                strcpy(tempStatus, bantu->status);
                tempTanggal = bantu->tanggal;
                tempBulan = bantu->bulan;
                tempTahun = bantu->tahun;
                tempJam = bantu->jam;
                tempMenit = bantu->menit;

                strcpy(bantu->namaTugas, bantu->next->namaTugas);
                strcpy(bantu->deskripsi, bantu->next->deskripsi);
                strcpy(bantu->status, bantu->next->status);
                bantu->tanggal = bantu->next->tanggal;
                bantu->bulan = bantu->next->bulan;
                bantu->tahun = bantu->next->tahun;
                bantu->jam = bantu->next->jam;
                bantu->menit = bantu->next->menit;

                strcpy(bantu->next->namaTugas, tempNama);
                strcpy(bantu->next->deskripsi, tempDeskripsi);
                strcpy(bantu->next->status, tempStatus);
                bantu->next->tanggal = tempTanggal;
                bantu->next->bulan = tempBulan;
                bantu->next->tahun = tempTahun;
                bantu->next->jam = tempJam;
                bantu->next->menit = tempMenit;

                swapped = true;
            }
            bantu = bantu->next;
        }
        akhir = bantu; // make sure elemen terakhir sudah pada tempatnya
    } while (swapped);

    simpanDataTugas(); // update file data_tugas.txt setelah sorting berdasarkan judul tugas
}

void searchTask()
{
    if (head == nullptr)
    {
        cout << "Belum ada tugas yang tersimpan.\n";
        return;
    }

    char keyword[100];
    cout << "[PASTIKAN PENULISAN NAMA TUGAS SAMA DENGAN PENULISAN PADA SAAT INPUT]\n";
    cout << "Masukkan nama tugas yang ingin dicari: ";
    cin.ignore();
    cin.getline(keyword, 100);

    tugas *bantu = head;
    bool found = false;

    cout << "\nHasil pencarian untuk \"" << keyword << "\":\n";
    cout << "========================================\n";

    while (bantu != nullptr)
    {
        if (strstr(bantu->namaTugas, keyword) != nullptr)
        {
            cout << "Nama Tugas : " << bantu->namaTugas << endl;
            cout << "Deskripsi  : " << bantu->deskripsi << endl;
            cout << "Status     : " << bantu->status << endl;
            cout << "Deadline   : " << bantu->tanggal << "/" << bantu->bulan << "/" << bantu->tahun
                 << " " << bantu->jam << ":" << bantu->menit << endl;
            cout << "----------------------------------------\n";
            found = true;
        }
        bantu = bantu->next;
    }

    if (!found)
    {
        cout << "Tugas \"" << keyword << "\" tidak ditemukan.\n";
    }
}
