namespace EDA_PRACTICA1;

using System;
using System.Diagnostics;
public class Program
{
    //cotador de milisegundos que tarda en hacer el proceso de leer, guardar y añadir los animales del csv
    public static void Main(string[] args)
    {
        Stopwatch sw = Stopwatch.StartNew();

        for (int i = 0; i < 1000; i++) { }

        sw.Stop();

        double microsegundos = sw.Elapsed.TotalMilliseconds * 1000;
        Console.WriteLine($"Tiempo transcurrido: {microsegundos} µs");
    }
}