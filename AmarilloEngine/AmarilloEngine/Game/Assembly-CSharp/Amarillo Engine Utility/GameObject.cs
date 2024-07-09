using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace AmarilloEngine
{
    public sealed class GameObject
    {
        public string name;
        public UIntPtr pointer;
        public Transform transform { get; set; }

        public GameObject()
        {
            name = "Empty";
            pointer = UIntPtr.Zero;
        }

        public GameObject(string _name, UIntPtr ptr, UIntPtr transPTR)
        {
            name = _name;
            pointer = ptr;
            transform = new Transform();
            transform.pointer = transPTR;
        }

        public extern string Name
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
        }

        public extern GameObject parent
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
        }

        public extern string Tag
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;

            [MethodImpl(MethodImplOptions.InternalCall)]
            set;
        }

        int GetHash()
        {
            return this.GetHashCode();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Vector3 GetForward();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Vector3 GetRight();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void SetVelocity(Vector3 velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void AddComponent(int componentType);
    }
}
