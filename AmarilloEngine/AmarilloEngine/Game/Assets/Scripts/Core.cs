using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using AmarilloEngine;

public class Core : AmarilloComponent
{
    public static Core instance;
   
    // Movment
    public float movementSpeed = 20f;

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
         

        }


		if (this.reference == null)
        {
            Debug.Log("[ERROR] Reference on Core.cs was 'null'");
            return;
        }
       
 
        if (InternalCalls.GetKey(AmarilloKeyCode.W) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetForward() * movementSpeed * Time.deltaTime;

        if (InternalCalls.GetKey(AmarilloKeyCode.S) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetForward() * -movementSpeed * Time.deltaTime;


        if (InternalCalls.GetKey(AmarilloKeyCode.A) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetRight() * -movementSpeed * Time.deltaTime;

        if (InternalCalls.GetKey(AmarilloKeyCode.D) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetRight() * movementSpeed * Time.deltaTime;

        //if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
        //    reference.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, rotationSpeed * Time.deltaTime);
        //if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
        //    reference.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, -rotationSpeed * Time.deltaTime);

        //Destroy current GameObject - It works
        if (InternalCalls.GetKey(AmarilloKeyCode.X) == KeyState.KEY_REPEAT)
            InternalCalls.Destroy(gameObject);

        //Create a GameObject - Not working
        if (InternalCalls.GetKey(AmarilloKeyCode.C) == KeyState.KEY_DOWN)
        {
            InternalCalls.CreateGameObject("Cube", Vector3.up);
            Debug.Log("Create 'Cube' GameObject");
        }


        //if (Input.GetMouseX() != 0 && turret != null)
        //    turret.localRotation = Quaternion.RotateAroundAxis(Vector3.up, -Input.GetMouseX() * mouseSens * Time.deltaTime) * turret.localRotation;

        ////if (Input.GetMouseY() != 0 && turret != null)
        ////    turret.localRotation = turret.localRotation * Quaternion.RotateAroundAxis(Vector3.right, -Input.GetMouseY() * Time.deltaTime);  

        Debug.Log("[WARNING] PosicionX " + gameObject.transform.localPosition.x);
        Debug.Log("[WARNING] PosicionY " + gameObject.transform.localPosition.y);
        Debug.Log("[WARNING] PosicionZ " + gameObject.transform.localPosition.z);
        return;
	}

    private void GameMovement()
    {
        //Debug.Log("[WARNING] PosicionX: " + x + "PosicionY: " + y);

        //if (x < 0)
        //{
        //    gameObject.transform.localPosition += reference.GetRight() * -movementSpeed * Time.deltaTime;
        //}
        //else if (x > 0)
        //{
        //    gameObject.transform.localPosition += reference.GetRight() * movementSpeed * Time.deltaTime;
        //}



        //gameObject.transform.localPosition.x += 20f * x * Time.deltaTime; ;
        //gameObject.transform.localPosition.y += 20f * y * Time.deltaTime; ;


        //gamepadInput = new Vector3(x, y, 0f);

        //if (y > 0)
        //{
        //    gameObject.transform.localPosition += new Vector3(0,0,1) * movementSpeed * Time.deltaTime;
        //}

        //if (IsMoving())
        //    inputList.Add(INPUT.IN_MOVE);
        //else if (currentState == STATE.MOVE && !IsMoving())
        //    inputList.Add(INPUT.IN_IDLE);

        //if (Input.GetGamepadRightTrigger() > 0)
        //    inputList.Add(INPUT.IN_SHOOT);

        //while (inputList.Count > 0)
        //{
        //    INPUT input = inputList[0];
        //    switch (currentState)
        //    {
        //        case STATE.NONE:
                    
        //            break;

        //        case STATE.IDLE:
        //            switch (input)
        //            {
        //                case INPUT.IN_MOVE:
        //                    currentState = STATE.MOVE;
                            
        //                    Move();
        //                    break;

        //                case INPUT.IN_DASH:
        //                    currentState = STATE.DASH;
        //                    Dash();
        //                    break;

        //                case INPUT.IN_SHOOTING:
        //                    currentState = STATE.SHOOT;
        //                    Shoot();
        //                    break;

        //            }
        //            break;
        //        case STATE.MOVE:
        //            switch (input)
        //            {
        //                case INPUT.IN_IDLE:
        //                    currentState = STATE.IDLE;
        //                    break;

        //                case INPUT.IN_DASH:
        //                    currentState = STATE.DASH;
        //                    Dash();
        //                    break;

        //                case INPUT.IN_SHOOTING:
        //                    currentState = STATE.SHOOT;
        //                    Shoot();
        //                    break;

        //                case INPUT.IN_DEAD:
        //                    break;
        //            }
        //            break;
        //        case STATE.SHOOT:
        //            switch (input)
        //            {
        //                case INPUT.IN_SHOOT_END:
        //                    currentState = STATE.SHOOT;
        //                    Shoot();
        //                    break;

        //                case INPUT.IN_DEAD:
        //                    break;
        //            }
        //            break;

        //        case STATE.DASH:
        //            switch (input)
        //            {
        //                case INPUT.IN_DASH_END:
        //                    currentState = STATE.IDLE;
        //                    break;
        //            }
        //            break;
        //    }
        //    inputList.RemoveAt(0);

        //}
    }

  
}






