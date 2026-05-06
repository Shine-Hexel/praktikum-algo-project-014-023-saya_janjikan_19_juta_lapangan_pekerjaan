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

int main()
{

    // tampil task
    char pilih;
    bool running = true;

    while (running)
    {
        tampilMenu();
        cin >> pilih;

        switch (toupper(pilih))
        {
        case 'A':
            // addTask();
            break;
        case 'B':
            // deleteTask();
            break;
        case 'C':
            // updateTask();
            break;
        case 'D':
            // searchTask();
            break;
        case 'E':
            // sortTask();
            break;
        case 'F':
            // doneTable();
            break;
        case 'H':
            // tandai tugas selesai
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
    tugas *bantu = node;
    fprintf(file, "%s;%s;%s;%d;%d;%d;%d;%d\n",
            bantu->namaTugas,
            bantu->deskripsi,
            bantu->status,
            bantu->tanggal,
            bantu->bulan,
            bantu->tahun,
            bantu->jam,
            bantu->menit);

    // jika tugas yang selesai adalah tugas paling atas
    if (bantu == head)
    {

        head = bantu->next;
        if (head != nullptr)
        {
            head->prev = nullptr;
        }
    }

    // jika tugas yang selesai adalah tugas paling bawah
    else if (bantu == tail)
    {
        tail = bantu->prev;
        if (tail != nullptr)
        {
            tail->next = nullptr;
        }
    }

    // jika tugas yang selesai berada di tengah list
    else
    {
        bantu->prev->next = bantu->next;
        bantu->next->prev = bantu->prev;
        bantu->next = nullptr;
        bantu->prev = nullptr;
    }

    // node bantu dihapus karena sudah tidak perlu lagi
    free(bantu);
    fclose(file);
}


void lihatTugasSelesai(){

    FILE *file = fopen("data_tugas_done.txt", "r");
    if (file == nullptr)
    {
        cout << "Tugas masih kosong" << endl;
        cout << endl;
        return;
    }

}



void cekStatusTerkini(tugas *node)
{
    // Implementasi pengecekan status tugas
    if (strcmp(node->status, "Done") == 0)
    {
        pindahTugasSelesai(node);
        return;
    }

    time_t saat_ini = time(0);

    tm deadline = {};
    deadline.tm_year = node->tahun - 1900; // tm_year dihitung sejak 1900
    deadline.tm_mon = node->bulan - 1;     // tm_mon dihitung dari 0 (Januari)
    deadline.tm_mday = node->tanggal;
    deadline.tm_hour = node->jam;
    deadline.tm_min = node->menit;
    deadline.tm_sec = 0;

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
        cekStatusTerkini(baru);

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
    }

    fclose(file);
    free(bantu);
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

    strcpy(baru->status, "In Progress");

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
