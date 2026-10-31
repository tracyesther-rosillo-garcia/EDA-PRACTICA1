using System.Collections.Generic;
using System.IO;

public class LeerCSV
{
    int animalesCargados; // número de animales cargados
    int animalesDescartados; // número de animales descartados
    private List<Animal> listaAnimales = new List<Animal>(); // lista de animales válidos

    
    public LeerCSV()
    {
        // Constructor
    }

    // Crea un nuevo animal a partir de una línea CSV
    public Animal CrearAnimal(string lineaCSV)
    {
        string[] datos = lineaCSV.Split(';');
        return new Animal(int.Parse(datos[1]), datos[2], datos[3], datos[4], int.Parse(datos[5]), float.Parse(datos[6]));
    }

    // Añade el animal a la lista y actualiza el contador
    public void AddAnimal(Animal animal)
    {
        listaAnimales.Add(animal);
        animalesCargados++;
    }

    // lee el csv proporcionado
    public void LeerArchivoCSV()
    {
        string linea;

        using (StreamReader sr = new StreamReader("Zooteca.csv"))
        {
            while ((linea = sr.ReadLine()) != null)
            {
                Animal animal = CrearAnimal(linea);

                if (!string.IsNullOrEmpty(Animal.Genero) && !string.IsNullOrEmpty(Animal.Clase))
                {
                    AddAnimal(animal);
                }
                else
                {
                    animalesDescartados++;
                }
            }
        }
    }
}