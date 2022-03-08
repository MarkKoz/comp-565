# COMP 565 - Assignment 2: Unity Mini Project

Course Title: Advanced Computer Graphics<br/>
Semester: Spring 2022<br/>

## Objective
In this project you are tasked to create a simple building block application. Here are the requirements and specifications to implement:

1. Have a base made of a cube with the following dimensions: `<10,0.1,10>` positioned at `<0,0,0>`
2. Create a simple UI to enable the user to select three different materials. Textures will be provided for the materials.
3. Create a simple UI to enable the user to change the type of geometry to place on the board dynamically.
    1. Unit Cube
    2. Unit Sphere
    3. Unit Capsule
4. Be able to delete a given object already placed on the base by exploding it.
5. You application will also visually guide the user displaying the selected geometry as a transparent yellowish material while they are deciding where to place their object.
6. You application will also visually display for the user when they can place/attach the selected geometry to an existing surface of another placed object.
7. Once the object is placed, it will apply the selected material.
8. Right-clicking an existing object will remove it from the scene by exploding the mesh. _Script will be provided._

## Sample Screenshots

<details>
<summary>Screenshot 1</summary>

![BuildingBlockNew-001](https://user-images.githubusercontent.com/1515135/157141503-7f12208b-832d-4e61-a1f1-6d6f665fbdac.png)
</details>
<details>
<summary>Screenshot 2</summary>

![BuildingBlockNew-003](https://user-images.githubusercontent.com/1515135/157141507-a522cf88-69f3-402a-ab1a-52383aa8a22d.png)
</details>
<details>
<summary>Screenshot 3</summary>

![BuildingBlockNew-004](https://user-images.githubusercontent.com/1515135/157141509-32b23a24-8993-4f9a-928f-796704623c3d.png)
</details>
<details>
<summary>Screenshot 4</summary>

![BuildingBlockNew-006](https://user-images.githubusercontent.com/1515135/157141513-47d94270-e7d5-4d22-9a94-27fffc955989.png)
</details>

## Initial Code

<details>
<summary>Explosion Script</summary>

```cs
using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class TriangleExplosion : MonoBehaviour
{
    public IEnumerator SplitMesh(bool destroy)
    {

        if (GetComponent<MeshFilter>() == null || GetComponent<SkinnedMeshRenderer>() == null)
        {
            yield return null;
        }

        if (GetComponent<Collider>())
        {
            GetComponent<Collider>().enabled = false;
        }

        Mesh M = new Mesh();
        if (GetComponent<MeshFilter>())
        {
            M = GetComponent<MeshFilter>().mesh;
        }
        else if (GetComponent<SkinnedMeshRenderer>())
        {
            M = GetComponent<SkinnedMeshRenderer>().sharedMesh;
        }

        Material[] materials = new Material[0];
        if (GetComponent<MeshRenderer>())
        {
            materials = GetComponent<MeshRenderer>().materials;
        }
        else if (GetComponent<SkinnedMeshRenderer>())
        {
            materials = GetComponent<SkinnedMeshRenderer>().materials;
        }

        Vector3[] verts = M.vertices;
        Vector3[] normals = M.normals;
        Vector2[] uvs = M.uv;
        for (int submesh = 0; submesh < M.subMeshCount; submesh++)
        {

            int[] indices = M.GetTriangles(submesh);

            for (int i = 0; i < indices.Length; i += 3)
            {
                Vector3[] newVerts = new Vector3[3];
                Vector3[] newNormals = new Vector3[3];
                Vector2[] newUvs = new Vector2[3];
                for (int n = 0; n < 3; n++)
                {
                    int index = indices[i + n];
                    newVerts[n] = verts[index];
                    newUvs[n] = uvs[index];
                    newNormals[n] = normals[index];
                }

                Mesh mesh = new Mesh();
                mesh.vertices = newVerts;
                mesh.normals = newNormals;
                mesh.uv = newUvs;

                mesh.triangles = new int[] { 0, 1, 2, 2, 1, 0 };

                GameObject GO = new GameObject("Triangle " + (i / 3));
                //GO.layer = LayerMask.NameToLayer("Particle");
                GO.transform.position = transform.position;
                GO.transform.rotation = transform.rotation;
                GO.AddComponent<MeshRenderer>().material = materials[submesh];
                GO.AddComponent<MeshFilter>().mesh = mesh;
                GO.AddComponent<BoxCollider>();
                Vector3 explosionPos = new Vector3(transform.position.x + UnityEngine.Random.Range(-0.5f, 0.5f), transform.position.y + UnityEngine.Random.Range(0f, 0.5f), transform.position.z + UnityEngine.Random.Range(-0.5f, 0.5f));
                GO.AddComponent<Rigidbody>().AddExplosionForce(UnityEngine.Random.Range(300, 500), explosionPos, 5);
                Destroy(GO, 5 + UnityEngine.Random.Range(0.0f, 5.0f));
            }
        }

        GetComponent<Renderer>().enabled = false;

        //yield return new WaitForSeconds(0.1f);
        if (destroy == true)
        {
            Destroy(gameObject, 0.1f);
        }

    }

    private void OnCollisionEnter(Collision collision)
    {
        if (collision.transform.tag.Equals("Base"))
            return;

        collision.transform.GetComponent<Renderer>().material.color = Color.red;
        Debug.Log(collision.transform.name);

        foreach (var cp in collision.contacts)
            Debug.Log(cp.point);
    }

    private void OnCollisionExit(Collision collision)
    {
        collision.transform.GetComponent<Renderer>().material.color = Color.white;
    }

    private void OnTriggerEnter(Collider other)
    {
        other.transform.GetComponent<Renderer>().material.color = Color.red;
    }

    private void OnTriggerExit(Collider other)
    {
        other.transform.GetComponent<Renderer>().material.color = Color.white;
    }
}
```
</details>
<details>
<summary>Sample from Class</summary>

```cs
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyMouseInput : MonoBehaviour
{
    int index = 0;

    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

        if (Input.GetMouseButtonUp(0))  // check if left button is pressed
        {
            // take mouse position, convert from screen space to world space, do a raycast, store output of raycast into
            // hitInfo object ...

            #region Screen To World
            RaycastHit hitInfo = new RaycastHit();
            bool hit = Physics.Raycast(Camera.main.ScreenPointToRay(Input.mousePosition), out hitInfo);
            if (hit)
            {

                #region HIDE
                var cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
                cube.tag = "MyCube";
                //cube.GetComponent<BoxCollider>().isTrigger = true;
                //cube.GetComponent<Renderer>().material = blockMaterial;
                #endregion

                //cube.transform.position = new Vector3(hitInfo.point.x, hitInfo.point.y + 0.5f, hitInfo.point.z);
                #region HIDE
                if (hitInfo.transform.tag.Equals("Base"))
                {
                    cube.transform.position = new Vector3(hitInfo.point.x, hitInfo.point.y + (0.5f), hitInfo.point.z);
                }
                #region HIDE
                else
                {
                    if (hitInfo.normal == new Vector3(0, 0, 1)) // z+
                    {
                        cube.transform.position = new Vector3(hitInfo.transform.position.x, hitInfo.transform.position.y, hitInfo.point.z + (0.5f));
                    }
                    #region HIDE
                    if (hitInfo.normal == new Vector3(1, 0, 0)) // x+
                    {
                        cube.transform.position = new Vector3(hitInfo.point.x + (0.5f), hitInfo.transform.position.y, hitInfo.transform.position.z);
                    }
                    if (hitInfo.normal == new Vector3(0, 1, 0)) // y+
                    {
                        cube.transform.position = new Vector3(hitInfo.transform.position.x, hitInfo.point.y + (0.5f), hitInfo.transform.position.z);
                    }
                    if (hitInfo.normal == new Vector3(0, 0, -1)) // z-
                    {
                        cube.transform.position = new Vector3(hitInfo.transform.position.x, hitInfo.transform.position.y, hitInfo.point.z - (0.5f));
                    }
                    if (hitInfo.normal == new Vector3(-1, 0, 0)) // x-
                    {
                        cube.transform.position = new Vector3(hitInfo.point.x - (0.5f), hitInfo.transform.position.y, hitInfo.transform.position.z);
                    }
                    if (hitInfo.normal == new Vector3(0, -1, 0)) // y-
                    {
                        cube.transform.position = new Vector3(hitInfo.transform.position.x, hitInfo.point.y - (0.5f), hitInfo.transform.position.z);
                    }
                    #endregion
                }
                #endregion

                //Debug.DrawRay(hitInfo.point, hitInfo.normal, Color.red, 2, false);
                //Debug.Log(hitInfo.normal);
                #endregion


            }
            else
            {
                Debug.Log("No hit");
            }
            #endregion
        }
    }
}
```
</details>

## Textures
Does not have to use these textures in particular.

<details>
<summary>Block Textures</summary>

|![MetalTexture](https://user-images.githubusercontent.com/1515135/157140563-8744b682-85c1-4197-a5d4-28d05cca1b3d.png)|![stone-blocks-texture](https://user-images.githubusercontent.com/1515135/157140571-caf0c67f-808e-45aa-af0e-fcf35c164c06.jpg)|![StoneBlocks](https://user-images.githubusercontent.com/1515135/157140574-1b804239-78ee-4785-8383-36486b8d7c5b.jpg)
|-|-|-|
</details>

<details>
<summary>UI Textures</summary>

|![CubeType](https://user-images.githubusercontent.com/1515135/157140581-4d6c72ae-d5fb-4980-bc3f-668ba4de70fa.png)|![SphereType](https://user-images.githubusercontent.com/1515135/157140584-267f1214-0c12-4f4c-83d7-b9b79c1823df.png)|![CapsuleType](https://user-images.githubusercontent.com/1515135/157140589-633142eb-3389-4d42-ba6a-47e16599b33b.png)
|-|-|-|
</details>

## Write-up
* Introduction
* Approach
* Challenges faced during the implementation
* Lessons learned

## Submission
Write-up and export of the Unity package.
