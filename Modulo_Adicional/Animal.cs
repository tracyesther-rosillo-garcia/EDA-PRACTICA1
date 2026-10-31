using System;
public class Animal
{
    //creacion clase animal con sus estadisticas
    public int Numero { get; set; }
    public string Clase { get; set; }
    public string Orden { get; set; }
    public string Genero { get; set; }
    public string Especie { get; set; }
    public int Peso { get; set; }
    public float Cerebro { get; set; }

    //constructor de la clase
    public Animal(int numero, string clase, string orden, string genero, string especie, int peso, float cerebro)
    {
        Numero = numero;
        Clase = clase;
        Orden = orden;
        Genero = genero;
        Especie = especie;
        Peso = peso;
        Cerebro = cerebro;
    }

    //mostrar la informacion de 1 animal
    public void MostrarInfo()
    {
        Console.WriteLine($"{Numero}, {Clase}, {Orden}, {Genero}, {Especie}, {Peso}, {Cerebro}");
    }

    //para el csv de animales
    public string ToCSV()
    {
        return $"Animal, {Numero}, {Clase}, {Orden}, {Genero}, {Especie}, {Peso}, {Cerebro}";
    }
}