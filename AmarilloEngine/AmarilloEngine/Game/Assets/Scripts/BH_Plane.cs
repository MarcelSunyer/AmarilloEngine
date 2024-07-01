using System;
using AmarilloEngine;
public class BH_Plane : AmarilloComponent
{
    public GameObject thisReference = null;
    public float speedIncrement = 0.03f;

    float cSpeed = 0.0f;

    public void Update()
    {
       
        if (Input.GetKey(AmarilloKeyCode.W) == KeyState.KEY_REPEAT && cSpeed < 0.18f)
            cSpeed += speedIncrement * Time.deltaTime;

        if ((Input.GetKey(AmarilloKeyCode.S) == KeyState.KEY_REPEAT || Input.GetKey(AmarilloKeyCode.W) == KeyState.KEY_IDLE) && cSpeed > 0.05f)
        {
            cSpeed -= speedIncrement * Time.deltaTime;
            if(cSpeed < 0.05f)
                cSpeed = 0.05f;
        }

        if (Input.GetKey(AmarilloKeyCode.A) == KeyState.KEY_REPEAT)
            thisReference.transform.localRotation = Quaternion.RotateAroundAxis(Vector3.up, 0.4f * Time.deltaTime) * thisReference.transform.localRotation;

        if (Input.GetKey(AmarilloKeyCode.D) == KeyState.KEY_REPEAT)
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

