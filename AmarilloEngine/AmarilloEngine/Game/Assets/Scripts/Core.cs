using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using AmarilloEngine;

public class Core : AmarilloComponent
{
    public static Core instance;
    

    enum STATE : int
    {
        NONE = -1,
        IDLE,
        MOVE,
        DASH,
        SHOOT
    }
    enum INPUT : int
    {
        IN_IDLE,
        IN_MOVE,
        IN_DASH,
        IN_DASH_END,
        IN_SHOOTING,
        IN_SHOOTING_END,
        IN_SHOOT,
        IN_SHOOT_END,
        IN_SEC_SHOOT,
        IN_DEAD
    }
    // Movment
    public float movementSpeed = 20f;
    
    //Controller Variables

    public GameObject reference = null;
	public GameObject turret = null;
	public GameObject shootPoint = null;
		
	
    public float mouseSens = 5.0f;

	public bool testBool = true;
	public string testString = "Juan";
    public bool start = true;

    public Vector3 testOtherClass; //Should find a way to tell if the class is a gameobject or not

	public void Update(/*int x*/)
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
       
 
        if (Input.GetKey(AmarilloKeyCode.W) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetForward() * movementSpeed * Time.deltaTime;
        if (Input.GetKey(AmarilloKeyCode.S) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetForward() * -movementSpeed * Time.deltaTime;

        if (Input.GetKey(AmarilloKeyCode.A) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetRight() * -movementSpeed * Time.deltaTime;
        if (Input.GetKey(AmarilloKeyCode.D) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetRight() * movementSpeed * Time.deltaTime;
        //if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
        //    reference.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, rotationSpeed * Time.deltaTime);
        //if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
        //    reference.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, -rotationSpeed * Time.deltaTime);

        //Destroy current GameObject - It works
        if (Input.GetKey(AmarilloKeyCode.X) == KeyState.KEY_REPEAT)
            InternalCalls.Destroy(gameObject);

        //Create a GameObject - Not working
        if (Input.GetKey(AmarilloKeyCode.C) == KeyState.KEY_DOWN)
        {
            InternalCalls.CreateGameObject("Cube", Vector3.up);
            Debug.Log("Create 'Cube' GameObject");
        }


        //if (Input.GetMouseX() != 0 && turret != null)
        //    turret.localRotation = Quaternion.RotateAroundAxis(Vector3.up, -Input.GetMouseX() * mouseSens * Time.deltaTime) * turret.localRotation;

        ////if (Input.GetMouseY() != 0 && turret != null)
        ////    turret.localRotation = turret.localRotation * Quaternion.RotateAroundAxis(Vector3.right, -Input.GetMouseY() * Time.deltaTime);

        if (Input.GetKey(AmarilloKeyCode.E) == KeyState.KEY_DOWN)
        {
            Debug.Log("Shoot!");
            //Debug.Log("[ERROR]" + gameObject.transform.localPosition.x);
            //Debug.Log("[ERROR]" + gameObject.transform.localPosition.z);
            //Vector3 pos = new Vector3(gameObject.transform.localPosition.x, 0, gameObject.transform.localPosition.z);
            //Vector3 rot = new Vector3(0, 1, 0);
            //Vector3 scale = new Vector3(1, 1, 1);
            //InternalCalls.CreateBullet(pos, rot, scale);
        }
        return;
	}
}






