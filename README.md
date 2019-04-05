# SoalShift_modul3_E15

## Nomor 1

Pada soal 1, dapat diselesaikan dengan menggunakan parameter yang di buat di main. Dimana terdapat 2 parameter 
yaitu count_param yang berisi banyaknya elemen yang ada pada char *my_param. Dalam my_param index ke 0
akan diisi dengan nama program dan dilanjut dengan parameter yang akan dipassing.

```
int main(int count_param, char *my_param[])
```

Lalu membuat thread sebanyak count_param-1 yang akan menghitung factorial. Dalam fungsi yang dijalankan oleh thread
perlu dicek terlebih dulu apakah id dengan thread yang masuk adalah sama. Jika tidak, maka thread yang tidak sama idnya
mencari sampai menemukan id yang sama baru menjalankan looping menghitung factorial.

```
void* factorial(void *arg)
{
    int a, iter, check;
    pthread_t id=pthread_self();                // Mengambil id dari thread yang sedang berjalan
    for(a=0;a<t;a++){
        if(pthread_equal(id, thread_id[a])){    // Mengecek id dari thread yang berjalan dengan id pada thread index ke a
			result[a]=1;
			for(iter=number[a]; iter>1; iter--) result[a]*=iter;
		}
    }
	return NULL;
}
```

Seteleh membuat thread maka harus dijoinkan supaya main menunggu semua thread selesai baru dijalankan sebuah program sort
untuk membuat outputan yang ada adalah terurut.

## Nomor 2
Pada soal 2, membuat 2 thread untuk setiap programnya(kecuali server pembeli cukup satu), yaitu thread untuk mengirim pesan
dan juga thread untuk menerima pesan. Server pembeli tidak perlu menggunakan thread karena program dari server pembeli tersebut
bersifat sequential dan tidak ada yang perlu dijalankan dengan paralel.

Untuk shared memory, terdapat pada permasalahan server penjual dan pembeli harus memiliki stock yang sama. Sehingga dibutuhkan 
sebuah shared memory.

```
    key_t key=1234;
    int shmid=shmget(key, sizeof(int), IPC_CREAT | 0666);
    data=shmat(shmid, NULL, 0);

    // Pembuatan socket

    // Pembuatan thread dan join

    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
```

Untuk socket, terdapat pada permasalahan untuk menghubungkan server dengan client baik penjual maupun pembeli. Penjual dan pembeli
memiliki socket berbeda.

## Nomor 5
