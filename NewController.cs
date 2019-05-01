using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class NewController : MonoBehaviour
{
    public static float speed;
    public float y, z, x;
    SerialPort serialPort = new SerialPort("COM3", 9600);
    public static float v, h, u_dreh, z_dreh, AcX, AcY, AcZ, tmp, GyX, GyY, GyZ;
    int b1, b2, jb;
    public Rigidbody RocketPrefab;
    public Transform ShipEnd;
    // Use this for initialization
    void Start()
    {
        serialPort.Open();
        serialPort.ReadTimeout = 100;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if (!serialPort.IsOpen)
        {
            serialPort.Open();
        }
        string DataString = serialPort.ReadLine();
        var dataBlocks = DataString.Split(',');

        if (!float.TryParse(dataBlocks[0], out v))
        {
            Debug.LogWarning("failed to parse");
            return;
        }
        if (!float.TryParse(dataBlocks[1], out h))
        {
            Debug.LogWarning("failed to parse 2");
            return;
        }
        if (!float.TryParse(dataBlocks[2], out u_dreh))
        {
            Debug.LogWarning("failed to parse 3");
            return;
        }
        if (!float.TryParse(dataBlocks[3], out z_dreh))
        {
            Debug.LogWarning("failed to parse 4");
            return;
        }
        if (!int.TryParse(dataBlocks[4], out b1))
        {
            Debug.LogWarning("failed to parse 5");
            return;
        }
        if (!int.TryParse(dataBlocks[5], out b2))
        {
            Debug.LogWarning("failed to parse 6");
            return;
        }
        if (!int.TryParse(dataBlocks[6], out jb))
        {
            Debug.LogWarning("failed to parse 7");
            return;
        }
        if (!float.TryParse(dataBlocks[7], out AcX))
        {
            Debug.LogWarning("failed to parse 8");
            return;
        }
        if (!float.TryParse(dataBlocks[7], out AcX))
        {
            Debug.LogWarning("failed to parse 8");
            return;
        }
        if (!float.TryParse(dataBlocks[8], out AcY))
        {
            Debug.LogWarning("failed to parse 9");
            return;
        }
        if (!float.TryParse(dataBlocks[9], out AcZ))
        {
            Debug.LogWarning("failed to parse 10");
            return;
        }
        if (!float.TryParse(dataBlocks[10], out tmp))
        {
            Debug.LogWarning("failed to parse 11");
            return;
        }
        if (!float.TryParse(dataBlocks[11], out GyX))
        {
            Debug.LogWarning("failed to parse 12");
            return;
        }
        if (!float.TryParse(dataBlocks[12], out GyY))
        {
            Debug.LogWarning("failed to parse 13");
            return;
        }
        if (!float.TryParse(dataBlocks[13], out GyZ))
        {
            Debug.LogWarning("failed to parse 14");
            return;
        }

        //Debug.Log("ax_" + AcX + "ay_" + AcY + "az_" + AcZ + "gx_" + GyX + "gy_" + GyY + "gz_" + GyZ);
        Debug.Log(u_dreh + "," + z_dreh);

        x = AcX / 190;
        y += GyY / 3000;
        z += GyX / 3000;

        transform.localEulerAngles = new Vector3(-x, -y, -z);

        if (h < -80)
        {
            transform.Translate(Vector3.left * Time.deltaTime * 5);
        }
        if (h > 80)
        {
            transform.Translate(Vector3.right * Time.deltaTime * 5);
        }

        //Geschwindigkeit fahren
        if (z_dreh == 0)
        {
            speed = 1;
        }
        if (z_dreh == 23)
        {
            speed = 2;
        }
        if (z_dreh == 45)
        {
            speed = 3;
        }
        if (z_dreh == 68)
        {
            speed = 4;
        }
        if (z_dreh == 90)
        {
            speed = 5;
        }
        if (z_dreh == 113)
        {
            speed = 6;
        }
        if (z_dreh == 135)
        {
            speed = 7;
        }
        if (z_dreh == 158)
        {
            speed = 8;
        }
        if (z_dreh == 158)
        {
            speed = 9;
        }
        if (z_dreh == 180)
        {
            speed = 10;
        }
        if (z_dreh == 203)
        {
            speed = 11;
        }

        if (z_dreh == 223)
        {
            speed = 12;
        }
        if (z_dreh == 248)
        {
            speed = 13;
        }
        if (z_dreh == 270)
        {
            speed = 14;
        }
        if (z_dreh == 293)
        {
            speed = 15;
        }
        if (z_dreh == 315)
        {
            speed = 16;
        }
        if (z_dreh == 338)
        {
            speed = 17;
        }

        transform.position += transform.forward * Time.deltaTime * speed;

        //Kugel werfen

        if (b1 == 1)
        {
            Rigidbody rocketInstance;
            rocketInstance = Instantiate(RocketPrefab, ShipEnd.position, ShipEnd.rotation) as Rigidbody;
            rocketInstance.AddForce(ShipEnd.forward * 500);
            Debug.Log("Wurf");
        }
        if(b2 == 1)
        {
            Application.Quit();
            Debug.Log("beenden");
        }
        if (jb == 1)
        {

        }
        
        

        }


}
