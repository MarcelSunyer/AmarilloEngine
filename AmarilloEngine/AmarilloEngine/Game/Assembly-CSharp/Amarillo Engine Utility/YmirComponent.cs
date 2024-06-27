using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace YmirEngine
{
    public enum ComponentType
    {
        NONE,
        TRANSFORM,
        MESH,
        MATERIAL,
        CAMERA,
        SCRIPT,
        AUDIO_SOURCE,
        AUDIO_LISTENER
    }
    public class YmirComponent
    {       

        public UIntPtr pointer;
        public ComponentType type;
        public static Dictionary<System.Type, ComponentType> componentTable = new Dictionary<Type, ComponentType> {
            { typeof(Transform), ComponentType.TRANSFORM},
        };


        public YmirComponent()
        {
            this.type = ComponentType.SCRIPT;
        }
        public extern GameObject gameObject
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        public ComponentType GetComponentType()
        {
            return type;
        }


        public int UID;
    }
}
