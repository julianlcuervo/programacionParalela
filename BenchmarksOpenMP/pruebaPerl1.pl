 #!/usr/bin/perl
if (@ARGV[0]) {
   $FInput = "$ARGV[0]";
} else {
   usage();
}
#Permite compilar los .c con la libreria openmp
system("gcc Benchmark1.c -o Benchmark1 -fopenmp");
system("gcc Benchmark2.c -o Benchmark2 -fopenmp");
system("gcc Benchmark3.c -o Benchmark3 -fopenmp");

#Se especifica que la ruta es en directorio actual
$PATH = "./";
#tamaño de las matrices
@SIZE = ("800","1100","1400","1700","2000");  
#iteraciones de los Benchmark
$Rep1 = 30;
$Rep2 = 30;
$Rep3 = 30;
#se define el archivo en la variable Nombre
$Nombre1 = ("Benchmark1");
$Nombre2 = ("Benchmark2");
$Nombre3 = ("Benchmark3");
         #Crea los archivos del primer Benchmark
         foreach $s (@SIZE) {
            #Nombre del archivo
            $file = "experimento-".$Nombre1."-".$s;
            #Guarda los datos el numero de veces de la iteración
            for ($k=0; $k<$Rep1; $k++) {
               system("$PATH$Nombre1 $s >>$file \n");
            }
            close($file);
         }
         #Crea los archivos del segundo Benchmark
         foreach $s (@SIZE) {
            #Nombre del archivo
            $file2 = "experimento-".$Nombre2."-".$s;
            #Guarda los datos el numero de veces de la iteración
            for ($k=0; $k<$Rep2; $k++) {
               system("$PATH$Nombre2 $s >>$file2 \n");
            }
            close($file2);
         }
         #Crea los archivos del tercer Benchmark
         foreach $s (@SIZE) {
            #Nombre del archivo
            $file3 = "experimento-".$Nombre3."-".$s;
            #Guarda los datos el numero de veces de la iteración
            for ($k=0; $k<$Rep3; $k++) {
               system("$PATH$Nombre3 $s >>$file3 \n");
            }
            close($file3);
         }


sub usage() {
        print "\n test.pl: Error incorrect usage \n\n";
        print "\t <directory> Is directory start point to check \n\n\n";
        exit(1);
}

