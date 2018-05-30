using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using Sebastian.Geometry;
using System;
using UnityEngine.UI;
using UnityEngine.EventSystems;

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
    public Shape activePoly;

    public void SetActiveObject(Shape obj)
    {
        activePoly = obj;
    }

    void Update()
    {
        PolyControl();
    }

    bool startRotate;
    Vector3 mousePos;

    void PolyControl()
    {
        if (Input.GetKeyDown(KeyCode.F))
        {
            mousePos = Input.mousePosition;
            startRotate = true;
        }
        if (Input.GetKeyUp(KeyCode.F))
        {
            startRotate = false;
        }
        if (Input.GetKey(KeyCode.F) && startRotate)
        {
            RotatePoly();
        }
    }
    void RotatePoly()
    {
        if (activePoly != null)
        {
            //activePoly.transform.Rotate(new Vector3(0, Input.mousePosition.y - mousePos.y, 0));
            //activePoly.gameObject.transform.RotateAroundLocal(new Vector3(activePoly.points[0].x, 0, activePoly.points[0].z), Input.mousePosition.y - mousePos.y);
            activePoly.gameObject.transform.RotateAround(activePoly.points[0], new Vector3(0, 1, 0), Input.mousePosition.y - mousePos.y);
            mousePos = Input.mousePosition;
        }
    }
#warning Todo: Add more colors
    public void InitColors()
    {
        colors.Add(Color.green);
        colors.Add(Color.blue);
        colors.Add(Color.yellow);
        colors.Add(Color.cyan);
        colors.Add(Color.magenta);
        colors.Add(Color.gray);
        colors.Add(Color.white);
        colors.Add(Color.red);
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

    void MarkObject(string[] input)
    {
        if (input.Length < 1) { return; }
        switch (input[1])
        {
            case "new":
                if (input.Length < 2) { return; }
                switch (input[2])
                {
                    case "point":
                        if (input.Length < 7) { return; }
                        string pointName = input[3];
                        string parentName = input[6];
                        string[] str = { "new", "point", parentName + "_" + pointName, input[7]};
                        Vector3 point = ReadMesh(str)[0];
                        Shifts.instance.marked = point; 
                        Shifts.instance.tasks.Add(new Shifts.Task(pointName, parentName, point));
                        break;
                    default:
                        break;
                }
                break;
            case "point":
                if (input.Length < 3) { return; }
                string name = input[2];
                Shifts.instance.tasks.Add(new Shifts.Task(name, false));
                break;
            case "clear":
                if (input.Length < 3) { return; }
                Shifts.instance.tasks.Add(new Shifts.Task(input[2], true));
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
                    if (buffer[2] == "along")
                    {
                        CharPoly(buffer);
                    }
                    else
                    {
                        MoveMesh(buffer);
                    }
                    break;
                case "mark":
                    MarkObject(buffer);
                    break;
                case "error":
                    ShowError(buffer);
                    break;
                case "messages":
                    break;
                default:
                    break;
            }
        }
        sr.Close();
    }
    void CharPoly(string[] buffer)
    {
        //move polyA along 2:-2 0:-1 -1:0 0:1 3:2 4:1 3:-1 then new poly charpolyputerAB
        GameObject obj = (GameObject)Instantiate(Resources.Load("Prefubs/Shape"));
        obj.name = buffer[buffer.Length - 1];
        obj.transform.position += new Vector3(0, Shapes.Count / 10f, 0);
        List<string> list = new List<string>();
        list.Add("new");
        list.Add("poly");
        list.Add(obj.name);
        for (int i = 3; i < buffer.Length; i++)
        {
            if (buffer[i] == "then") { break; }
            list.Add(buffer[i]);
        }
        Shapes.Add(new Shape(ReadMesh(list.ToArray()), obj.name));
        CreateToogleButton(Shapes[Shapes.Count - 1]);
        Shifts.instance.tasks.Add(new Shifts.Task(Shapes[Shapes.Count - 1]));
        //move polyA along charpolyputerAB then new poly 2:-2 0:-1 -1:0 0:1 3:2 4:1 3:-1
        for (int i = 1; i < Shapes[Shapes.Count - 1].points.Count; i++)
        {
            //move poly polyA to 1:0
            string[] str = { "move", "poly", buffer[1], "to", Shapes[Shapes.Count - 1].points[i].x + ":" + Shapes[Shapes.Count - 1].points[i].z };
            MoveMesh(str);
            Shifts.instance.tasks.Add(new Shifts.Task(Shapes[Shapes.Count - 1], true));
        }
        string[] str2 = { "move", "poly", buffer[1], "to", Shapes[Shapes.Count - 1].points[0].x + ":" + Shapes[Shapes.Count - 1].points[0].z };
        MoveMesh(str2);
        Shifts.instance.tasks.Add(new Shifts.Task(Shapes[Shapes.Count - 1], true));
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
                if (!float.TryParse(coordinate, out buf[j]))
                {
                    Debug.Log("parse " + coordinate + " failed");
                }
                //buf[j] = (float)System.Convert.ToDouble(coordinate);
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

#warning Todo: add support to creating chain and points.
    void CreateNewMesh(string[] buffer)
    {
        GameObject obj = (GameObject)Instantiate(Resources.Load("Prefubs/Shape"));
        obj.name = buffer[2];
        obj.transform.position += new Vector3(0, Shapes.Count/10f, 0);
        Shapes.Add(new Shape(ReadMesh(buffer), buffer[2]));
        CreateToogleButton(Shapes[Shapes.Count - 1]);
        Shifts.instance.tasks.Add(new Shifts.Task(Shapes[Shapes.Count - 1]));
    }

    int toogleCount = 0;
    void CreateToogleButton(Shape shape)
    {
        GameObject button = (GameObject)Instantiate(Resources.Load("Prefubs/Toogle"));
        button.transform.SetParent(GameObject.Find("Canvas").transform);
        button.transform.position = new Vector2(110, 578 - 50*toogleCount);
        button.name = "Toogle " + toogleCount;
        button.GetComponentInChildren<Text>().text = shape.name;
        toogleCount++;
    }
    public void OnPointerClick(PointerEventData pointerEventData)
    {
        if (pointerEventData.button == PointerEventData.InputButton.Left)
        {
            Debug.Log(pointerEventData.selectedObject.gameObject.name);
        }
    }
}

