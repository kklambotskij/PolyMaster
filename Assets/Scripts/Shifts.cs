using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Sebastian.Geometry;
using System;
using UnityEngine.UI;

public class Shifts : MonoBehaviour {

    //Making this script global and editable
    private static Shifts _instance;
    public static Shifts instance
    {
        get
        {
            if (_instance == null)
                _instance = Shifts.FindObjectOfType<Shifts>();
            return _instance;
        }
    }

    public float globalSpeed = 0.1f;
    public void ChangeGlobalSpeed()
    {
        globalSpeed = GameObject.Find("Canvas").transform.Find("InputSpeed").GetComponent<Slider>().value;
    }

    public bool pause;
    
    public class Task
    {
        public enum Type {Move, Shape, Mark}
        public Type type;

        //Create shape or Move Shape
        public Shape shape;
        public Movement move;
        public bool done;

        public Task(Shape shape)
        {
            done = false;
            type = Type.Shape;
            this.shape = shape;
        }
        public Task(Movement move)
        {
            type = Type.Move;
            done = false;
            this.move = move;
        }

        //Mark object
        public string markName;
        public string markParent;
        public int markIndex;

        public Task(string markName, string markParent, int markIndex)
        {
            done = false;
            type = Type.Mark;
            this.markName = markName;
            this.markParent = markParent;
            this.markIndex = markIndex;
        }
    }

    public class Movement
    {
        public GameObject gameObject;
        public Vector3 shift;
        public float spd;
        public bool hard;

        public Movement(Movement move)
        {
            gameObject = move.gameObject;
            this.shift = move.shift + this.gameObject.transform.position;
            spd = move.spd;
        }

        public Movement(Shape poly, Vector3 shift)
        {
            gameObject = poly.gameObject;
            this.shift = shift + poly.gameObject.transform.position;
            spd = 1;
        }

        public Movement(Shape poly, Vector3 shift, float speed, bool hard)
        { 
            if (poly.gameObject != null)
            {
                gameObject = poly.gameObject;
                shift += new Vector3(0, gameObject.transform.position.y, 0);
                this.hard = hard;
                if (hard)
                {
                    this.shift = shift - poly.points[0];
                }
                else
                {
                    this.shift = shift + new Vector3(poly.gameObject.transform.position.x, 0, poly.gameObject.transform.position.z);
                }
            }
            else
            {
                Debug.Log("GameObject of Shape" + poly.name + "Not found");
            }
            spd = speed;
        }
    }

    int currentTask;
    bool moving;
    public List<Task> tasks = new List<Task>();
    public List<Movement> moves = new List<Movement>();

    void NextTask()
    {
        if(currentTask < -1 || currentTask > tasks.Count - 2) { return; }
        currentTask++;
        StartCoroutine(Example());
    }
    //Awake starts before Start()
    private void Awake()
    {
        moving = false;
        pause = true;
        currentTask = -1;
    }

    private void Start()
    {
        NextTask();
    }

    public void Shift(string name, Vector3 shift, float speed, bool hard)
    {
        int index = Loader.instance.FindShape(name);
        if (index < 0) { return; }
        if (Loader.instance.Shapes[index].gameObject == null) { return; }
        moves.Add(new Movement(Loader.instance.Shapes[index], shift, speed, hard));
        tasks.Add(new Task(moves[moves.Count - 1]));
    }
    void MoveProcessing()
    {
        if (moving && !pause)
        {
            if (moves.Count < 1) { return; }
            if (moves[0].gameObject.transform.position == moves[0].shift)
            {
                Debug.Log(String.Concat("Move to ", moves[0].shift.x, " ", moves[0].shift.z, " complete"));
                moves.RemoveAt(0);
                if (moves.Count < 1) { return; }
                if (moves[0].hard == false)
                {
                    moves[0].shift = new Vector3(moves[0].gameObject.transform.position.x, moves[0].gameObject.transform.position.y, moves[0].gameObject.transform.position.z) + moves[0].shift;
                }
                moving = false;
                NextTask();
            }
            moves[0].gameObject.transform.position = Vector3.MoveTowards(moves[0].gameObject.transform.position, moves[0].shift, maxDistanceDelta: globalSpeed * Time.deltaTime);
        }
    }

    void Update()
    {
        MoveProcessing();
    }

    IEnumerator Example()
    {
        yield return new WaitForSeconds(1/globalSpeed);
        if(pause) { StartCoroutine(Example()); }
        else
        {
            switch (tasks[currentTask].type)
            {
                case Task.Type.Move:
                    moving = true;
                    break;
                case Task.Type.Shape:
                    moving = false;
                    RunTimeCreator rtc = tasks[currentTask].shape.gameObject.transform.Find("Shape Creator").GetComponent<RunTimeCreator>();
                    rtc.LoadShape();
                    if (tasks[currentTask].shape.name.StartsWith("char"))
                    {
                        rtc.CreateBorder();
                        rtc.HideShape();
                    }
                    NextTask();
                    break;
                case Task.Type.Mark:
#warning Todo: add marked objects by task
#if false
                    GameObject point = GameObject.Find(tasks[currentTask].markParent);
                    point = point.transform.Find("point " + tasks[currentTask].markIndex).gameObject;
                    if (point != null)
                    {
                        point.name = tasks[currentTask].markName;
                        point.GetComponent<MeshRenderer>().material.color = Color.red;
                    }
#endif
                    NextTask();
                    break;
            }
        }
    }
}