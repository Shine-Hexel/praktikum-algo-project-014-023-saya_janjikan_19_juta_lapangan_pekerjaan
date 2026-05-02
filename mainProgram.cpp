#include <iostream>
#include <ctime>
using namespace std;

struct tugas
{
    string namaTugas;
    string deskripsi;
    string status;
    int deadline;
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
