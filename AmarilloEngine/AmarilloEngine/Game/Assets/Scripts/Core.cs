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

    //private bool testBool = true;
    //private string testString = "Juan";
    private bool start = true;

    //Vector3 position = new Vector3(1f, 0f, 0f); // Ejemplo de posición inicial (1, 0, 0)
    //Vector3 rotationAngles = new Vector3(0f, 90f, 0f); // Ángulos de rotación (0 grados en X, 90 grados en Y, 0 grados en Z)
    //private float rotationAmount = 5f;

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
        if (Input.GetKey(AmarilloKeyCode.W) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += new Vector3(0, 0, 1) / 2;
        if (Input.GetKey(AmarilloKeyCode.S) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += new Vector3(0, 0, -1) / 2;

        if (Input.GetKey(AmarilloKeyCode.A) == KeyState.KEY_REPEAT)
        {
            gameObject.transform.localRotation *= Quaternion.RotateAroundAxis(new Vector3(1,5,2), 10);//RotateVector(position, rotationAngles, rotationAmount) ;
        }
        if (Input.GetKey(AmarilloKeyCode.D) == KeyState.KEY_REPEAT)
        {
             //gameObject.transform.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, -1 * Time.deltaTime);
        }

        return;
	}
}






