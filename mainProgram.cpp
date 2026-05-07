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
void pindahTugasSelesai(tugas *node);
void lihatTugasSelesai();
void aturStatusTerkini(tugas *node);
void simpanDataTugas();
void ngambilDataTugas();
void lihatTugas();
void lihatTugasSelesai();
void nambahTugasBaru();
void sortingTugasDL();
void sortingTugasJudul();
void searchTask();

int main()
{
    lihatTugas();
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
            // deleteTask();
            break;
        case 'C':
            // updateStatusTask();
            break;
        case 'D':
            searchTask();
            break;
        case 'E':
            char pilihSort;
            cout << "\nPilih metode sorting:\n";
            cout << "1. Sorting by Deadline (DL)\n";
            cout << "2. Sorting by Nama Tugas (A-Z)\n";
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
                cout << "\nPilihan sorting tidak valid!\nSilahkan pilih antara opsi 1 atau 2.\n\n";
            break;
        case 'F':
            lihatTugasSelesai();
            break;
        case 'H':
            tandaiTugasSelesai();
            break;
        case 'G':
            cout << "\n  Selamat fokus menyelesaikan tugas! Data tersimpan secara otomatis >_< \n\n";
            running = false;
            break;
        default:
            cout << "\nPilihan tidak valid!\nSilahkan pilih antara opsi A-G.\n";
        }
    }
}

void tampilMenu()
{
    cout << "========================================\n";
    cout << "|              MENU UTAMA              |\n";
    cout << "========================================\n";
    cout << "    [A] Tambah Tugas Baru\n";
    cout << "    [B] Hapus Tugas\n";
    cout << "    [C] Update Status Tugas\n";
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

    fprintf(file, "%s;%s;%s;%d;%d;%d;%d;%d\n",
            node->namaTugas,
            node->deskripsi,
            node->status,
            node->tanggal,
            node->bulan,
            node->tahun,
            node->jam,
            node->menit);

    // jika tugas yang selesai adalah tugas paling atas
    if (node == head)
    {
        head = node->next;
        if (head != nullptr)
        {
            head->prev = nullptr;
        }
    }

    // jika tugas yang selesai adalah tugas paling bawah
    else if (node == tail)
    {
        tail = node->prev;
        if (tail != nullptr)
        {
            tail->next = nullptr;
        }
    }

    // jika tugas yang selesai berada di tengah list
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        node->next = nullptr;
        node->prev = nullptr;
    }

    // node bantu dihapus karena sudah tidak perlu lagi
    free(node);
    fclose(file);
}

void tandaiTugasSelesai()
{
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
            pindahTugasSelesai(bantu);
            cout << "\nTugas \"" << namaTugas << "\" berhasil ditandai selesai!\n\n";
            found = true;
            break;
        }
        bantu = bantu->next;
    }

    if (!found)
    {
        cout << "\nTugas \"" << namaTugas << "\" tidak ditemukan.\n\n";
    }
}


void aturStatusTerkini(tugas *node)
{
    time_t saat_ini = time(0);

    tm deadline = {};

    deadline.tm_year = node->tahun - 1900; // tm_year dihitung sejak 1900
    deadline.tm_mon = node->bulan - 1;     // tm_mon dihitung dari 0 (Januari)
    deadline.tm_mday = node->tanggal;
    deadline.tm_hour = node->jam;
    deadline.tm_min = node->menit;
    //deadline.tm_sec = 0;

    time_t waktuDeadline = mktime(&deadline);

    if (waktuDeadline < saat_ini)
    {
        strcpy(node->status, "Late");
    }
    else
    {
        strcpy(node->status, "In Progress");
    }
}

void simpanDataTugas()
{
    // buat nyimpen data tugas ke file
    FILE *file = fopen("data_tugas.txt", "w");
    tugas *bantu = head;

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

void ngambilDataTugas()
{
    // buat ngambil data tugas dari file
    FILE *file = fopen("data_tugas.txt", "r");
    if (file == nullptr)
    {
        cout << "Tugas masih kosong" << endl;
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

        // status tugas diupdate sesuai dengan waktu sekarang
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

    ngambilDataTugas();
    int angka = 0;

    tugas *bantu = new tugas;
    bantu = head;
    cout << "========================================\n";
    cout << "|              TUGAS SAYA              |\n";
    cout << "========================================\n";

    while (bantu != nullptr)
    {
        cout << "Nama Tugas : " << bantu->namaTugas << endl;
        cout << "Deskripsi  : " << bantu->deskripsi << endl;
        cout << "Status     : " << bantu->status << endl;
        cout << "Deadline   : " << bantu->tanggal << "/" << bantu->bulan << "/" << bantu->tahun
             << " " << bantu->jam << ":" << bantu->menit << endl;
        cout << "----------------------------------------\n";
        cout << endl;
        bantu = bantu->next;
    }
}

void lihatTugasSelesai()
{

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
                  &menit) != EOF)
    {
        cout << "Nama Tugas : " << namaTugas << endl;
        cout << "Deskripsi  : " << deskripsi << endl;
        cout << "Status     : " << status << endl;
        cout << "Deadline   : " << tanggal << "/" << bulan << "/" << tahun
             << " " << jam << ":" << menit << endl;
        cout << "----------------------------------------\n";
        cout << endl;
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

    cout << "Masukkan tanggal deadline (1-31): ";
    cin >> baru->tanggal;

    cout << "Masukkan bulan deadline (1-12): ";
    cin >> baru->bulan;

    cout << "Masukkan tahun deadline: ";
    cin >> baru->tahun;

    cout << "Masukkan jam deadline (0-23): ";
    cin >> baru->jam;

    cout << "Masukkan menit deadline (0-59): ";
    cin >> baru->menit;

    aturStatusTerkini(baru);
    
    baru->next = nullptr;
    baru->prev = nullptr;

    if (head == nullptr)
    {
        head = tail = baru;
    }
    else
    {
        tail->next = baru;
        baru->prev = tail;
        tail = baru;
    }

    simpanDataTugas();
}

void sortingTugasDL()
{
    if (head == nullptr || head->next == nullptr)
        return; // jumlah list hanya 1 atau kosong

    tugas *bantu = head;

    while (bantu != nullptr)
    {
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

    simpanDataTugas();
}

// sorting by judul tugas secara alfabetis menggunakan bubble sort
void sortingTugasJudul()
{
    if (head == nullptr || head->next == nullptr)
        return; // jumlah list hanya 1 atau kosong

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

    simpanDataTugas();
}

void searchTask()
{
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
