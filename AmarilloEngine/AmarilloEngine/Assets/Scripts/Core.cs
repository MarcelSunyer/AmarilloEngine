using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;



public class Core : YmirComponent
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

    private STATE currentState = STATE.NONE;
    private List<INPUT> inputList = new List<INPUT>();
    // Movment
    public float movementSpeed = 20f;
    private double angle = 0.0f;
    
    //Controller Variables

    Vector3 gamepadInput;

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
            currentState = STATE.IDLE;
            Debug.Log("[WARNING] testString: " + testString);
            reference = gameObject;

           start = false;
         
            Debug.Log("[WARNING] Name" + gameObject.Name);
                
            //testString = reference.Tag;
            testString = "Update string";
            string name = "hola";
            Vector3 pos = new Vector3(0, 1, 0);
   
                InternalCalls.CreateGameObject(name, pos);

        }


		if (this.reference == null)
        {
            Debug.Log("[ERROR] Reference on Core.cs was 'null'");
            return;
        }
       
 
        if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetForward() * movementSpeed * Time.deltaTime;
        if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetForward() * -movementSpeed * Time.deltaTime;

        if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetRight() * -movementSpeed * Time.deltaTime;
        if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += gameObject.GetRight() * movementSpeed * Time.deltaTime;
        //if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
        //    reference.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, rotationSpeed * Time.deltaTime);
        //if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
        //    reference.localRotation *= Quaternion.RotateAroundAxis(Vector3.up, -rotationSpeed * Time.deltaTime);

        //Destroy current GameObject - It works
        if (Input.GetKey(YmirKeyCode.X) == KeyState.KEY_REPEAT)
            InternalCalls.Destroy(gameObject);

        //Create a GameObject - Not working
        if (Input.GetKey(YmirKeyCode.C) == KeyState.KEY_DOWN)
        {
            InternalCalls.CreateGameObject("Cube", Vector3.up);
            Debug.Log("Create 'Cube' GameObject");
        }


        //if (Input.GetMouseX() != 0 && turret != null)
        //    turret.localRotation = Quaternion.RotateAroundAxis(Vector3.up, -Input.GetMouseX() * mouseSens * Time.deltaTime) * turret.localRotation;

        ////if (Input.GetMouseY() != 0 && turret != null)
        ////    turret.localRotation = turret.localRotation * Quaternion.RotateAroundAxis(Vector3.right, -Input.GetMouseY() * Time.deltaTime);

        if (Input.GetKey(YmirKeyCode.E) == KeyState.KEY_DOWN)
        {
            Debug.Log("Shoot!");
            Debug.Log("[ERROR]" + gameObject.transform.localPosition.x);
            Debug.Log("[ERROR]" + gameObject.transform.localPosition.z);
            Vector3 pos = new Vector3(gameObject.transform.localPosition.x, 0, gameObject.transform.localPosition.z);
            Vector3 rot = new Vector3(0, 1, 0);
            Vector3 scale = new Vector3(1, 1, 1);
            InternalCalls.CreateBullet(pos, rot, scale);
        }


        float x = Input.GetLeftAxisX();
        float y = Input.GetLeftAxisY();

       


        gamepadInput = new Vector3(x, y, 0f);

        Debug.Log("[WARNING] InputX " + gamepadInput.x);
        Debug.Log("[WARNING] InputY " + gamepadInput.y);


        if (gamepadInput.x > 0)
        {
            gameObject.transform.localPosition += gameObject.GetRight() * movementSpeed * Time.deltaTime;
        }
        if (gamepadInput.x < 0)
        {
            gameObject.transform.localPosition += gameObject.GetRight() * -movementSpeed * Time.deltaTime;
        }
        if (gamepadInput.y > 0)
        {
            gameObject.transform.localPosition += gameObject.GetForward() * -movementSpeed * Time.deltaTime;
        }
        if (gamepadInput.y < 0)
        {
            gameObject.transform.localPosition += gameObject.GetForward() * movementSpeed * Time.deltaTime;
        }


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

    private void UpdateState()
    {
        switch (currentState)
        {
            case STATE.NONE:
                break;
            case STATE.IDLE:
                break;
            case STATE.MOVE:
                RotatePlayer();
                gameObject.transform.localPosition += reference.GetForward() * movementSpeed * Time.deltaTime;
                Debug.Log("[ERROR]Mover");
                break;
            case STATE.DASH:
                
                break;
            case STATE.SHOOT:
                
                break;
            default:
                Debug.Log("NEED TO ADD STATE TO CORE");
                break;
        }
    }

    private void RotatePlayer()
    {
        //Calculate player rotation
        Vector3 aX = new Vector3(gamepadInput.x, 0, -gamepadInput.y - 1);
        Vector3 aY = new Vector3(0, 0, 1);
        aX = Vector3.Normalize(aX);

        if (aX.x >= 0)
        {
            angle = Math.Acos(Vector3.Dot(aX, aY) - 1);
        }
        else if (aX.x < 0)
        {
            angle = -Math.Acos(Vector3.Dot(aX, aY) - 1);
        }

        //Convert angle from world view to orthogonal view
        angle += 0.785398f; //Rotate 45 degrees to the right

        gameObject.transform.localRotation = Quaternion.RotateAroundAxis(Vector3.up, (float)-angle);
    }

    private void Move()
    {
         // Play Audio and SetAnimation;
    }
    private void Dash()
    {
        // Play Audio and SetAnimation;
    }
    private void Shoot()
    {
        // Play Audio and SetAnimation;
    }

    private bool IsMoving()
    {
     
        return Math.Abs( gamepadInput.x) > 0 || Math.Abs(gamepadInput.y) > 0;
    }
}






