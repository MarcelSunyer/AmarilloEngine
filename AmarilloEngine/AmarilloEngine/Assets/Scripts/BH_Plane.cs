using System;
using YmirEngine;


public class BH_Plane : YmirComponent
{
    public GameObject thisReference = null;
    public float speedIncrement = 0.03f;

    float cSpeed = 0.0f;

    public void Update()
    {
       
        if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT && cSpeed < 0.18f)
            cSpeed += speedIncrement * Time.deltaTime;

        if ((Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT || Input.GetKey(YmirKeyCode.W) == KeyState.KEY_IDLE) && cSpeed > 0.05f)
        {
            cSpeed -= speedIncrement * Time.deltaTime;
            if(cSpeed < 0.05f)
                cSpeed = 0.05f;
        }

        if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
            thisReference.transform.localRotation = Quaternion.RotateAroundAxis(Vector3.up, 0.4f * Time.deltaTime) * thisReference.transform.localRotation;

        if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
            thisReference.transform.localRotation = Quaternion.RotateAroundAxis(Vector3.up, -0.4f * Time.deltaTime) * thisReference.transform.localRotation;

        if (Input.GetMouseX() > 0)
            thisReference.transform.localRotation = thisReference.transform.localRotation * Quaternion.RotateAroundAxis(Vector3.forward, Input.GetMouseX() * 0.1f * Time.deltaTime);

        if (Input.GetMouseX() < 0)
            thisReference.transform.localRotation = thisReference.transform.localRotation * Quaternion.RotateAroundAxis(Vector3.forward, Input.GetMouseX() * 0.1f * Time.deltaTime);

        if (Input.GetMouseY() > 0)
            thisReference.transform.localRotation = thisReference.transform.localRotation * Quaternion.RotateAroundAxis(Vector3.right, -Input.GetMouseY() * 0.1f * Time.deltaTime);

        if (Input.GetMouseY() < 0)
            thisReference.transform.localRotation = thisReference.transform.localRotation * Quaternion.RotateAroundAxis(Vector3.right, -Input.GetMouseY() * 0.1f * Time.deltaTime);

        thisReference.transform.localPosition += thisReference.GetForward() * cSpeed;
    }




}

