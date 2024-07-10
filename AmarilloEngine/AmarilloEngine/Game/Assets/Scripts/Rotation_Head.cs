using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using AmarilloEngine;

public class Rotation_Head : AmarilloComponent
{
    private GameObject reference = null;

    private bool start = true;
    public void Update()
    {
        //Hardcoceado para que sea una especie de "Awake()"
        if (start)
        {
            reference = gameObject;
            start = false;
        }

        if (this.reference == null)
        {
            Debug.Log("[ERROR] Reference on Core.cs was 'null'");
            return;
        }

        //Los controles esan alreves pq me dava pereza arreglar el rotate de la camara jiji
        // Controles de velocidad
        if (Input.GetMouseX() != 0)
        {
            float mouseX = Input.GetMouseX();
            float rotationSpeed = 0.1f; // Ajusta la velocidad de rotación según sea necesario
            float angleY = mouseX * rotationSpeed * Time.deltaTime;
            Quaternion deltaRotationY = Quaternion.RotateAroundAxis(Vector3.up, angleY);
            gameObject.transform.localRotation = deltaRotationY * gameObject.transform.localRotation;
        }
        return;
    }


}


