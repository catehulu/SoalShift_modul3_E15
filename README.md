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

## Nomor 3

Nomor 3 meminta sebuah program dengan beberapa input string dan beberapa fitur.
Disini perfitur dijadikan thread, dengan local variabel tersendiri. Juga disediakan global variable yang dijadikan sebagai flag untuk kontrol antar thread.
```
pthread_t a,s; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread
int status = 1;
int mode = 3;
int counterA = 1;
int counterB = 1;
```
status adalah status apakah program sudah selesai atau belum, mode adalah mode inputan dari user, counter digunakan sebagai timer 10 deitk ketika fungsi terkunci, dalam program ini tiap thread diberikan counter untuk masing-masing fitur.

Pertama-tama program menginisialisasi thread
```
 char fitur1[] = "All Status";
    char fitur2[] = "Agmal Ayo Bangun";
    char fitur3[] = "Iraj Ayo Tidur";
    char *input = malloc(50*sizeof(char));
    int err;
    pthread_create(&(a), NULL, &agmal, NULL);
    pthread_create(&(s), NULL, &iraj, NULL);

```

setelah itu program menunggu inputan user. Tergantung dari inputannya , akan ada beberapa hal yang berbeda akan terjadi.

Untuk tiap thradnya kurang lebih sama, hanya berbeda variabel.

```
void* agmal(void *arg)
{
    int WakeUp_Status = 0;
    int counter = 0;
    pthread_t local_thread;
    //printf("Thread a created\n");
    while(1){
        while(mode != 0 && mode != 1 && status);
        if (mode == 0) {
            printf("Agmal WakeUp_Status = %d\n",WakeUp_Status);
            sleep(1);
        }
        else if (!counterA)
        {
            printf("Agmal Ayo Bangun disabled 10 s\n");
            mode = 3;
        }
        else if (mode == 1){
            WakeUp_Status += 15;
            counter++;
            if (WakeUp_Status >= 100) {
                printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
                pthread_cancel(s);
                status = 0;
                return NULL;

            }
            
            if (counter == 3) {
                pthread_create(&(local_thread), NULL, counter1, NULL);
                counterB = 0;
                counter = 0;
            }

            mode = 3;
        }
        
    }
    
}
```
 Jika tidak berada dalam mode yang benar, maka thread akan menunggu. Jika thread dalam fitur All Status, maka kedua thread akan memprint status masing-masing. Jika thread dalam kondisi fitur penambahan/pengurangan status, maka local variabel akan di tambah. lalu counter internal akan bertambah. Jika counter internal sama dengan 3, maka counter global yang dijadikan sebagai flag akan dijadikan 0. Dan thread untuk mengubah flag tersebut kembali menjadi 1 akan berjalan selamam 10 detik.
 ```
 void* counter1(void *arg)
{
    sleep(10);
    counterB = 1;

}
 ```

 Jika program selesai, maka thread akan diselesaikan, dan thread yang berlawanan akan di close juga melalui thread yang selesai.

## Nomor 4

Dalam program ini yang dilakukan hanyalah dengan system. Terdapat 7 thread, 2 thread membuat file txt, 2 thread melakukan zip file, dan 2 thread melakukan unzip, dan 1 thread melakukan wait selama 15 detik.

```
void* createlist(void *arg)
{
	pthread_t id=pthread_self();
	if(pthread_equal(id,tid_list[0])) 
	{
		DIR* dir = opendir("/home/catehulu/Documents/FolderProses1");
		if(dir == NULL) system("mkdir ~/Documents/FolderProses1");
		closedir(dir);
		system("ps -aux | head -n11 >> ~/Documents/FolderProses1/SimpanProses1.txt");
	}
	else if(pthread_equal(id,tid_list[1])) 
	{
		DIR* dir = opendir("/home/catehulu/Documents/FolderProses2");
		if(dir == NULL) system("mkdir ~/Documents/FolderProses2");
		closedir(dir);
		system("ps -aux | head -n11 >> ~/Documents/FolderProses2/SimpanProses2.txt");
	}

	pthread_exit(NULL);
```
Ini adalah program yang melakukan pembuatan file txt. Dicek apakah direktori sudah ada atau tidak, jika tidak direktori akan dibuat. Hasil dari ps -aux dimasukkan ke file SimpanProses. Diambil 11 line karena maksimal 10 proses yang diapatkan.

```
void* zipfile(void *arg)
{
	pthread_t id=pthread_self();
	if(pthread_equal(id,tid_zip[0])) 
	{
		system("zip -j ~/Documents/FolderProses1/KompresProses1.zip ~/Documents/FolderProses1/SimpanProses1.txt >> /dev/null");
		system("rm ~/Documents/FolderProses1/SimpanProses1.txt  >> /dev/null");
	}
	else if(pthread_equal(id,tid_zip[1])) 
	{
		system("zip -j ~/Documents/FolderProses2/KompresProses2.zip ~/Documents/FolderProses2/SimpanProses2.txt  >> /dev/null");
		system("rm ~/Documents/FolderProses2/SimpanProses2.txt  >> /dev/null");
	}

	pthread_exit(NULL);

}
```

Ini adalah program yang melakukan proses zip file. Dalam proses ini file dizip dan file aslinya dihapus.

```
void* wait_timer(void *arg)
{
	printf("Menunggu 15 detik untuk ekstrak kembali\n");
	sleep(15);
	pthread_exit(NULL);
}
```
Ini adalah program yang melakukan proses wait. Dalam proses ini hanya disleep selama 15 detik.
```
void* unzip(void *arg)
{
	pthread_t id=pthread_self();
	if(pthread_equal(id,tid_unzip[0])) 
	{
		system("unzip ~/Documents/FolderProses1/KompresProses1.zip -d ~/Documents/FolderProses1/  >> /dev/null");
	}
	else if(pthread_equal(id,tid_unzip[1])) 
	{
		system("unzip ~/Documents/FolderProses2/KompresProses2.zip -d ~/Documents/FolderProses2/  >> /dev/null");
	}

	pthread_exit(NULL);
}
```
Ini adalah proses uang melakukan proses unzip. File diunzip pada folder yang sama.

```
nt main(void)
{
	int err;
	for(int i = 0; i < 2; i++)
	{
		err=pthread_create(&(tid_list[i]),NULL,&createlist,NULL);
		if (err) printf("Error when creating thread : [%s]",strerror(err));
	}
	
	pthread_join(tid_list[0],NULL);
	pthread_join(tid_list[1],NULL);

	for(int i = 0; i < 2; i++)
	{
		err=pthread_create(&(tid_zip[i]),NULL,&zipfile,NULL);
		if (err) printf("Error when creating thread : [%s]",strerror(err));
	}
	
	pthread_join(tid_zip[0],NULL);
	pthread_join(tid_zip[1],NULL);
	
	err=pthread_create(&(tid_timer),NULL,&wait_timer,NULL);
	pthread_join(tid_timer,NULL);

	for(int i = 0; i < 2; i++)
	{
		err=pthread_create(&(tid_unzip[i]),NULL,&unzip,NULL);
		if (err) printf("Error when creating thread : [%s]",strerror(err));
	}

	pthread_join(tid_unzip[0],NULL);
	pthread_join(tid_unzip[1],NULL);
	
	exit(0);
	return 0;
}
```

Ini adalah proses utama. jadi pada tiap tahap dilakukan pemanggilan thread. Setelah itu dilakukan join tiap thread.

## Nomor 5

Pada soal 5, didalam code game terdapat 6 thread yang menangani tugas sendiri sendiri.
Masing masing thread mempunyai tugas menampilkan UI, ready untuk menerima inputan,
untuk menghitung cooldown kamar mandi, menghitung regen health dari pet, menghitung hunger, dan juga mengitung hygiene.
Sedangkan dalam code market terdapat 2 thread yang menangani inputan dan outputan.

Alasan dibuat thread tersendiri karena setiap thread memiliki kegunaan tersendiri yang tidak dapat
diganggu oleh kebutuhan yang lain. Contohnya untuk UI pada shop harus menampilkan jumlah barang saat ini,
tanpa menunggu inputan dari user. Bisa saja pemain pada game membeli stok pada toko, akan tetapi pada
code shop(market) tetap menunggu inputan sehingga outputan jumlah barang pada saat itu tidak sesuai
dengan kenyataan bahwa item dari toko telah dibeli oleh pemain dalam game.

Dalam soal 5 ini juga membutuhkan shared memory, dimana mengharuskan sebuah data pada stok item di shop juga merupakan
jumlah yang sama saat ditampilkan dishop dari sudut pandang pemain. Sehingga shared memory dibutuhkan pada soal nomor 5 ini.
