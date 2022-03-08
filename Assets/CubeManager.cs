using System;
using System.Collections.Generic;

using UnityEngine;
using UnityEngine.EventSystems;

public class CubeManager : MonoBehaviour
{
    public UIManager ui;

    private Camera mainCamera;
    private GameObject activeGuide;
    private IReadOnlyDictionary<PrimitiveType, GameObject> guides;
    private IReadOnlyDictionary<UIManager.Texture, Material> materials;

    private void Start()
    {
        mainCamera = Camera.main;

        if (mainCamera is null)
            throw new ApplicationException("An enabled main camera was not found.");

        materials = new Dictionary<UIManager.Texture, Material>
        {
            {
                UIManager.Texture.StoneBricks01,
                Resources.Load<Material>("Materials/stone_bricks01")
            },
            {
                UIManager.Texture.StoneBricks02,
                Resources.Load<Material>("Materials/stone_bricks02")
            },
            { UIManager.Texture.Metal01, Resources.Load<Material>("Materials/metal01") },
        };

        guides = new Dictionary<PrimitiveType, GameObject>
        {
            { PrimitiveType.Cube, transform.GetChild(0).gameObject },
            { PrimitiveType.Sphere, transform.GetChild(1).gameObject },
            { PrimitiveType.Capsule, transform.GetChild(2).gameObject },
        };
        activeGuide = guides[ui.primitive];
    }

    private void Update()
    {
        if (EventSystem.current.IsPointerOverGameObject()) return;

        bool hit = Physics.Raycast(mainCamera.ScreenPointToRay(Input.mousePosition), out var hitInfo);

        if (!hit) return;

        if (Input.GetMouseButtonDown(0))
        {
            CreatePrimitive(hitInfo);
        }
        else if (Input.GetMouseButtonDown(1))
        {
            if (!hitInfo.transform.tag.Equals("Base"))
            {
                var explosion = hitInfo.transform.GetComponentInParent<TriangleExplosion>();
                StartCoroutine(explosion.SplitMesh(true));
            }
        }
        else
        {
            ShowGuide(hitInfo);
        }
    }

    private void ShowGuide(RaycastHit hitInfo)
    {
        var newGuide = guides[ui.primitive];
        newGuide.transform.position = GetNewPosition(hitInfo);

        var colour = hitInfo.transform.tag.Equals("Base") ? Color.yellow : Color.green;
        if (IsColliding(newGuide.transform.position))
            colour = Color.red;

        colour.a = 0.5f;
        newGuide.GetComponent<MeshRenderer>().material.color = colour;

        activeGuide.SetActive(false);
        activeGuide = newGuide;
        activeGuide.SetActive(true);
    }

    private void CreatePrimitive(RaycastHit hitInfo)
    {
        var position = GetNewPosition(hitInfo);

        if (IsColliding(position))
            return;

        var primitive = GameObject.CreatePrimitive(ui.primitive);
        primitive.GetComponent<MeshRenderer>().material = materials[ui.texture];
        primitive.transform.position = position;
        primitive.layer = 3;
        primitive.AddComponent<TriangleExplosion>();

        // Create a box collider for non-cube primitives.
        if (!(ui.primitive is PrimitiveType.Cube))
        {
            var child = AddChildBoxCollider(primitive);

            // Create a second collider for capsules, since they're 2x in height.
            if (ui.primitive is PrimitiveType.Capsule)
            {
                child.transform.localPosition = Vector3.down / 2;
                var child2 = AddChildBoxCollider(primitive);
                child2.transform.localPosition = Vector3.up / 2;
            }
        }
    }

    private Vector3 GetNewPosition(RaycastHit hitInfo)
    {
        Vector3 pos;

        if (hitInfo.transform.tag.Equals("Base"))
            pos = hitInfo.point + (Vector3.up / 2);
        else
            pos = hitInfo.transform.position + hitInfo.normal;

        if (ui.primitive is PrimitiveType.Capsule)
            pos.y += 0.5f;

        return pos;
    }

    private bool IsColliding(Vector3 position)
    {
        float radius = 0.5f - Physics.defaultContactOffset;
        const int layerMask = 1 << 3;

        return ui.primitive switch
        {
            PrimitiveType.Sphere => Physics.CheckSphere(position, radius, layerMask),
            PrimitiveType.Capsule => Physics.CheckCapsule(
                position + (Vector3.up / 2),
                position + (Vector3.down / 2),
                radius,
                layerMask),
            _ => Physics.CheckBox(position, Vector3.one * radius, Quaternion.identity, layerMask)
        };
    }

    private static GameObject AddChildBoxCollider(GameObject parent)
    {
        var child = new GameObject();
        child.name = "BoxCollider";
        child.transform.parent = parent.transform;
        child.transform.localPosition = Vector3.zero;
        child.layer = 6;

        var boxCollider = child.AddComponent<BoxCollider>();
        boxCollider.size = Vector3.one;

        return child;
    }
}
