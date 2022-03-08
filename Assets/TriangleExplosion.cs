using UnityEngine;

using System.Collections;
using System;

public class TriangleExplosion : MonoBehaviour
{
    public IEnumerator SplitMesh(bool destroy)
    {
        if (GetComponent<MeshFilter>() == null || GetComponent<SkinnedMeshRenderer>() == null)
            yield return null;

        if (GetComponent<Collider>())
            GetComponent<Collider>().enabled = false;

        var m = new Mesh();

        if (GetComponent<MeshFilter>())
            m = GetComponent<MeshFilter>().mesh;
        else if (GetComponent<SkinnedMeshRenderer>())
            m = GetComponent<SkinnedMeshRenderer>().sharedMesh;

        var materials = Array.Empty<Material>();

        if (GetComponent<MeshRenderer>())
            materials = GetComponent<MeshRenderer>().materials;
        else if (GetComponent<SkinnedMeshRenderer>())
            materials = GetComponent<SkinnedMeshRenderer>().materials;

        var verts = m.vertices;
        var normals = m.normals;
        var uvs = m.uv;

        for (int submesh = 0; submesh < m.subMeshCount; submesh++)
        {
            int[] indices = m.GetTriangles(submesh);

            for (int i = 0; i < indices.Length; i += 3)
            {
                var newVerts = new Vector3[3];
                var newNormals = new Vector3[3];
                var newUvs = new Vector2[3];

                for (int n = 0; n < 3; n++)
                {
                    int index = indices[i + n];
                    newVerts[n] = verts[index];
                    newUvs[n] = uvs[index];
                    newNormals[n] = normals[index];
                }

                var mesh = new Mesh
                {
                    vertices = newVerts,
                    normals = newNormals,
                    uv = newUvs,
                    triangles = new[] { 0, 1, 2, 2, 1, 0 }
                };

                var obj = new GameObject("Triangle " + (i / 3));
                obj.transform.position = transform.position;
                obj.transform.rotation = transform.rotation;
                obj.layer = 2; // Ignore ray casts.
                obj.AddComponent<MeshRenderer>().material = materials[submesh];
                obj.AddComponent<MeshFilter>().mesh = mesh;
                obj.AddComponent<BoxCollider>();

                var explosionPos = new Vector3(
                    transform.position.x + UnityEngine.Random.Range(-0.5f, 0.5f),
                    transform.position.y + UnityEngine.Random.Range(0f, 0.5f),
                    transform.position.z + UnityEngine.Random.Range(-0.5f, 0.5f));

                obj.AddComponent<Rigidbody>().AddExplosionForce(UnityEngine.Random.Range(300, 500), explosionPos, 5);
                Destroy(obj, 5 + UnityEngine.Random.Range(0.0f, 5.0f));
            }
        }

        GetComponent<Renderer>().enabled = false;

        // yield return new WaitForSeconds(0.1f);
        if (destroy == true)
            Destroy(gameObject, 0.1f);
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
