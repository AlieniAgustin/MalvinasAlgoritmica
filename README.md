Este proyecto desarrolla un software para la Asociación de Veteranos de la Guerra de Malvinas "Virgen del Rosario" que permite gestionar la información de hasta 250 veteranos de guerra. Implementado en C, el sistema administra datos personales y ofrece una serie de funciones como:

    Añadir una persona, manteniendo un orden ascendente por apellido, nombre y DNI.
    Eliminar una persona por DNI.
    Modificar los datos personales de una persona.
    Listar a todas las personas o buscar por apellido.
    Mostrar personas fallecidas.
    Filtrar por fuerza de pertenencia (Marina, Gendarmería, Ejército o Aeronáutica).
    Listar personas nacidas en un mes específico, ordenando por fecha de cumpleaños.

Toda la información se almacena en el archivo malvinas.dat, que se actualiza al cerrar el programa y se lee al iniciar para recuperar los datos existentes.
Estructura del Proyecto

Cada persona se almacena con los siguientes datos:

    Identificación: DNI, nombre, apellido
    Datos personales: fecha de nacimiento, ciudad actual y en la guerra, dirección, correo electrónico, teléfono, estado de vida o fallecimiento
    Detalles adicionales: fuerza, destino, función, grado, y secuelas de guerra

El proyecto utiliza algoritmos de ordenamiento y búsqueda, como el método de burbuja para ordenar datos y búsqueda secuencial para encontrar y modificar personas.
Menú de Opciones

    Insertar personas
    Eliminar personas
    Modificar datos de personas
    Listar todas las personas
    Buscar por apellido
    Listar personas fallecidas
    Filtrar por fuerza de pertenencia
    Listar cumpleaños por mes
    Salir (actualiza el archivo malvinas.dat)
