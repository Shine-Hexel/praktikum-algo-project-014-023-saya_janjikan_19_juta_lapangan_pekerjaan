#include <iostream>
using namespace std;
void tampilMenu();

int main(){
	char pilih;
	bool running = true;
	
	while (running){
	tampilMenu();
	cin >> pilih;
	
	    switch (toupper(pilih)) {
			case 'A':
				//addTask();
				break;
			case 'B':
				//deleteTask();
				break;
			case 'C':
				//updateTask();
				break;
			case 'D':
				//searchTask();
				break;
			case 'E':
				//sortTask();
				break;
			case 'F':
				//doneTable();
				break;
			case 'G':
				cout << "\n  Selamat fokus menyelesaikan tugas! Data tersimpan secara otomatis >_< \n\n";
				running = false;
				break;
			default:
				cout << "\n  [!] Pilihan tidak valid.\n";
		}
	}
}

void tampilMenu(){
	cout << "========================================\n";
    cout << "|              MENU UTAMA              |\n";
    cout << "========================================\n";
    cout << "    [A] Tambah Tugas Baru\n";
    cout << "    [B] Hapus Tugas\n";
    cout << "    [C] Update Status Tugas\n";
    cout << "    [D] Cari Tugas\n";
    cout << "    [E] Sorting Tugas\n";
    cout << "    [F] Lihat Tugas Done\n";
    cout << "    [G] Logout\n";
    cout << "========================================\n";
    cout << "  Pilihan: ";
}
