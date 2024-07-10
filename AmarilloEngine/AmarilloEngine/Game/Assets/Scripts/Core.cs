using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using AmarilloEngine;

public class Core : AmarilloComponent
{
    //private static Core instance;

    // Movment
    //private float movementSpeed = 20f;
    //private float rotation = 20f;

    //Controller Variables

    private GameObject reference = null;
    //public GameObject turret = null;
    //public GameObject shootPoint = null;


    //private float mouseSens = 5.0f;

    private bool start = true;
    public float speedIncrement = 6f;
    public float maxSpeed = 30f;
    public float minSpeed = 0f;
    public float rotationSpeed = 2.0f;

    float cSpeed = 0.0f;

    public void Update()
	{        
        //Hardcoceado para que sea una especie de "Awake()"
        if (start)
        {
            reference = gameObject;

        }

		if (this.reference == null)
        {
            Debug.Log("[ERROR] Reference on Core.cs was 'null'");
            return;
        }

        //Los controles esan alreves pq me dava pereza arreglar el rotate de la camara jiji
        // Controles de velocidad
        if (Input.GetKey(AmarilloKeyCode.W) == KeyState.KEY_REPEAT)
        {
            cSpeed += speedIncrement * Time.deltaTime;
            if (cSpeed > maxSpeed)
                cSpeed = maxSpeed;
        }
        if (Input.GetKey(AmarilloKeyCode.S) == KeyState.KEY_REPEAT)
        {
            cSpeed -= speedIncrement * Time.deltaTime;
            if (cSpeed < minSpeed)
                cSpeed = minSpeed;
        }

        // Controles de rotación
        if (Input.GetKey(AmarilloKeyCode.A) == KeyState.KEY_REPEAT)
        {
            gameObject.transform.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, rotationSpeed * Time.deltaTime);
        }

        if (Input.GetKey(AmarilloKeyCode.D) == KeyState.KEY_REPEAT)
        {
            gameObject.transform.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, -rotationSpeed * Time.deltaTime);
        }

        // Movimiento hacia adelante
        gameObject.transform.localPosition += gameObject.transform.GetForward() * cSpeed * Time.deltaTime;

        return;
    }
       
	
}






