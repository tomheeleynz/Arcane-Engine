using System;

namespace Arcane
{
    public class Entity
    {
        public Entity()
        {

        }

        public static void CreateEntity(int id)
        {
            Console.WriteLine($"Calling Method from C++: {id}");
        }
    }
}