using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using Sebastian.Geometry;
using System;
using UnityEngine.UI;

public class Loader : MonoBehaviour {

    //Making this script global and editable
    private static Loader _instance;
    public static Loader instance
    {
        get
        {
            if (_instance == null)
                _instance = Loader.FindObjectOfType<Loader>();
            return _instance;
        }
    }

    public List<Color> colors = new List<Color>();
    
    public void InitColors()
    {
        colors.Add(Color.green);
        colors.Add(Color.blue);
        colors.Add(Color.yellow);
        colors.Add(Color.cyan);
        colors.Add(Color.magenta);
        colors.Add(Color.gray);
        colors.Add(Color.white);
    }

    private void Awake()
    {
        Load();
    }
	
    public List<Shape> Shapes = new List<Shape>();
    public void Load ()
    {
        Shifts.instance.moves.Clear();
        for (int i = 0; i < Shapes.Count; i++)
        {
            Destroy(Shapes[i].gameObject);
        }
        Shapes.Clear();
        Read();
        using (StreamWriter sw = File.CreateText("log.txt")) { sw.Close(); }
        foreach (var item in Shapes)
        {
            WriteMesh(item);
        }
	}
    //To do
    void MarkObject(string[] input)
    {
        switch (input[1])
        {
            case "new":
                string name = input[3];
                string parentName = input[6];
                int index = 0;
                if (input[7] != "custom")
                {
                    index = Convert.ToInt32(input[7]);
                }
                Shifts.instance.tasks.Add(new Shifts.Task(name, parentName, index));
                break;
            case "clear":
                break;
            default:
                break;
        }
    }
    //Parser
    void Read()
    {
        StreamReader sr = new StreamReader("output.txt");
        string stroka;
        while ((stroka = sr.ReadLine()) != null)
        {
            string[] buffer = stroka.Split(' ');
            switch (buffer[0])
            {
                case "new":
                    CreateNewMesh(buffer);
                    break;
                case "move":
                    MoveMesh(buffer);
                    break;
                case "mark":
                    MarkObject(buffer);
                    break;
                case "error":
                    ShowError(buffer);
                    break;
                default:
                    break;
            }
        }
        sr.Close();
    }

    void ShowError(string[] input)
    {
        if(input.Length < 2) { return; }
        string error = input[1];
        for (int i = 2; i < input.Length; i++)
        {
            error = String.Concat(error, " ", input[i]);
        }
        GameObject.Find("Canvas").transform.Find("Error").transform.Find("Text").GetComponent<Text>().text = error;
        GameObject.Find("Canvas").transform.Find("Error").gameObject.SetActive(true);
    }

    void MoveMesh(string[] input)
    {
        //move poly polyM from 2:1 to 0:0
        List<Vector3> verts = new List<Vector3>();
        for (int i = 4; i < input.Length; i+=2)
        {
            string[] coordinates = input[i].Split(':');
            float[] buf = new float[4];
            int j = 0;
            foreach (var coordinate in coordinates)
            {
                buf[j] = (float)System.Convert.ToDouble(coordinates[j]);
                j++;
            }
            verts.Add(new Vector3(buf[0], 0, buf[1]));
        }
        Vector3 vector3 = new Vector3(0, 0, 0);
        bool hard = false;
        switch (input[3])
        {
            case "from":
                if (verts.Count < 2) { return; }
                vector3 = verts[1] - verts[0];
                break;
            case "to":
                if (verts.Count < 1) { return; }
                vector3 = verts[0];
                hard = true;
                break;
            default:
                return;
        }
        Shifts.instance.Shift(input[2], vector3, 1, hard);
    }

    public List<Vector3> ReadMesh(string[] input)
    {
        //0 - new
        //1 - poly
        //2 - name
        //3, 4, 5, 6 verts
        //new poly charPoly 2:0 2:1 3.5:1 3:0
        List<Vector3> verts = new List<Vector3>();
        for (int i = 3; i < input.Length; i++)
        {
            string[] coordinates = input[i].Split(':');
            float[] buf = new float[4]; 
            int j = 0;
            foreach(var coordinate in coordinates)
            {
                buf[j] = (float)System.Convert.ToDouble(coordinate);
                j++;
            }
            if(buf.Length < 2) { continue; }
            verts.Add(new Vector3(buf[0], 0, buf[1]));
        }
        return verts;
    }

    int WriteMesh(Shape poly)
    {
        //Check for 256 path length!!!
#if false
        string path = String.Concat("LOG", DateTime.Now.ToString("yyyy.MM.dd HH.mm.ss"), ".txt");
        using (StreamWriter sw = File.AppendText(path))
#else
        using (StreamWriter sw = File.AppendText("log.txt"))
#endif
        {
            string stroka = "new poly " + poly.name;
            foreach (var item in poly.points)
            {
                stroka = String.Concat(stroka, " ", item.x, ":", item.z);
            }
            sw.WriteLine(stroka);
            sw.Close();
        }
        return 0;
    }

    public int FindShape(string name)
    {
        int poly = -1;
        for (int i = 0; i < Shapes.Count; i++)
        {
            if (Shapes[i].name == name)
            {
                poly = i;
                break;
            }
        }
        return poly;
    }

    void CreateNewMesh(string[] buffer)
    {
        GameObject obj = (GameObject)Instantiate(Resources.Load("Prefubs/Shape"));
        obj.name = buffer[2];
        obj.transform.position += new Vector3(0, Shapes.Count/10f, 0);
        Shapes.Add(new Shape(ReadMesh(buffer), buffer[2]));
        Shifts.instance.tasks.Add(new Shifts.Task(Loader.instance.Shapes[Shapes.Count - 1]));
    }
}
