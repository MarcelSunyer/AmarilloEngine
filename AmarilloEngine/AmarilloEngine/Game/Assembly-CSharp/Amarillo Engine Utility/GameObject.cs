using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace AmarilloEngine
{
    public sealed class GameObject
    {
        public string name;
        public UIntPtr pointer;
        public Transform transform;
        
        public GameObject()
        {
            name = "Empty";
            pointer = UIntPtr.Zero;
        }
        public GameObject(string _name, UIntPtr ptr)
        {
            name = _name;
            pointer = ptr;
            transform = new Transform();


        }

        public extern string Name
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }
        public extern GameObject parent
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        } 
      
        public extern string Tag
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }





        int GetHash()
        {
            return this.GetHashCode();
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetForward();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetRight();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void SetVelocity(Vector3 velocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void AddComponent(int componentType);

    }
}
