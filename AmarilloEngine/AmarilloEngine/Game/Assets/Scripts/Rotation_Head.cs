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
        // Hardcodeado para que sea una especie de "Awake()"
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

        if (Input.GetMouseX() != 0)
        {
            float mouseX = Input.GetMouseX();
            float rotationSpeed = 0.4f;
            float angleY = mouseX * rotationSpeed * Time.deltaTime;
            Quaternion deltaRotationY = Quaternion.RotateAroundAxis(Vector3.up, angleY);
            gameObject.transform.localRotation = deltaRotationY * gameObject.transform.localRotation;
        }

       
        if (Input.GetMouseClick(MouseButton.LEFT) == KeyState.KEY_DOWN)
        {
            ShootBullet();
        }
    }

    private void ShootBullet()
    {
        // Crear la bala y establecer su rotación y posición según el Rotation_Head
        Vector3 bulletPosition = gameObject.transform.globalPosition; 
        Quaternion bulletRotation = gameObject.transform.globalRotation; 
        Vector3 bulletScale = new Vector3(0.1f, 0.1f, 0.1f);
        bulletPosition.y += 2.5f;
        InternalCalls.CreateBullet(bulletPosition, bulletRotation, bulletScale);
    }


}


