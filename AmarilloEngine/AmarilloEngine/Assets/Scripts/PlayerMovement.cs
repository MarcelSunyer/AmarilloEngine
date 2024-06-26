using System;
using System.Collections.Generic;
using YmirEngine;

public class PlayerMovement : YmirComponent
{

    Vector3 gamepadInput;

    public GameObject thisReference = null;

    public float movementSpeed = 5f;

    public void Update()
    {
        thisReference = gameObject;

        //-------------- Keyboard --------------//

        //if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
        //    gameObject.transform.localPosition += gameObject.GetForward() * movementSpeed * Time.deltaTime;
        //if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
        //    gameObject.transform.localPosition += gameObject.GetForward() * -movementSpeed * Time.deltaTime;

        //if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
        //    gameObject.transform.localPosition += gameObject.GetRight() * -movementSpeed * Time.deltaTime;
        //if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
        //    gameObject.transform.localPosition += gameObject.GetRight() * movementSpeed * Time.deltaTime;

        if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += new Vector3(0, 0, 1) * movementSpeed * Time.deltaTime;
        if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += new Vector3(0, 0, 1) * -movementSpeed * Time.deltaTime;

        if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += new Vector3(1, 0, 0) * movementSpeed * Time.deltaTime;
        if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += new Vector3(1, 0, 0) * -movementSpeed * Time.deltaTime;

        if (Input.GetKey(YmirKeyCode.E) == KeyState.KEY_DOWN)
        {
            Debug.Log("Shoot!");
            Vector3 pos = new Vector3(gameObject.transform.localPosition.x, 0, gameObject.transform.localPosition.z);
            Vector3 rot = new Vector3(0, 1, 0);
            Vector3 scale = new Vector3(0.2f, 0.2f, 0.2f);
            InternalCalls.CreateBullet(pos, rot, scale);
        }

        //-------------- Controller --------------//

        float x = Input.GetLeftAxisX();
        float y = Input.GetLeftAxisY();

        gamepadInput = new Vector3(x, y, 0f);

        //if (gamepadInput.x > 0)
        //{
        //    gameObject.transform.localPosition += gameObject.GetRight() * -movementSpeed * Time.deltaTime;
        //}
        //if (gamepadInput.x < 0)
        //{
        //    gameObject.transform.localPosition += gameObject.GetRight() * movementSpeed * Time.deltaTime;
        //}
        //if (gamepadInput.y > 0)
        //{
        //    gameObject.transform.localPosition += gameObject.GetForward() * -movementSpeed * Time.deltaTime;
        //}
        //if (gamepadInput.y < 0)
        //{
        //    gameObject.transform.localPosition += gameObject.GetForward() * movementSpeed * Time.deltaTime;
        //}

        if (gamepadInput.x > 0)
        {
            gameObject.transform.localPosition += new Vector3(1,0,0) * -movementSpeed * Time.deltaTime;
        }
        if (gamepadInput.x < 0)
        {
            gameObject.transform.localPosition += new Vector3(1, 0, 0) * movementSpeed * Time.deltaTime;
        }
        if (gamepadInput.y > 0)
        {
            gameObject.transform.localPosition += new Vector3(0, 0, 1) * -movementSpeed * Time.deltaTime;
        }
        if (gamepadInput.y < 0)
        {
            gameObject.transform.localPosition += new Vector3(0, 0, 1) * movementSpeed * Time.deltaTime;
        }

        if (Input.IsGamepadButtonBPressedCS())
        {
            Debug.Log("Shoot!");
            Vector3 pos = new Vector3(gameObject.transform.localPosition.x, 0, gameObject.transform.localPosition.z);
            Vector3 rot = new Vector3(0, 1, 0);
            Vector3 scale = new Vector3(0.2f, 0.2f, 0.2f);
            InternalCalls.CreateBullet(pos, rot, scale);
            Input.GameControllerRumbleCS(3,32,100);
        }
    }
}