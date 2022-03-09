using System;
using UnityEngine;

public class UIManager : MonoBehaviour
{
    public enum Texture
    {
        StoneBricks01,
        StoneBricks02,
        Metal01,
    }

    public Texture texture = Texture.StoneBricks01;
    public PrimitiveType primitive = PrimitiveType.Cube;

    public void OnTextureClick(int index)
    {
        texture = IntToEnum<Texture>(index);
    }

    public void OnPrimitiveClick(int index)
    {
        primitive = IntToEnum<PrimitiveType>(index);
    }

    private static T IntToEnum<T>(int index)
        where T : Enum
    {
        if (Enum.IsDefined(typeof(T), index))
            return (T) Enum.ToObject(typeof(T), index);

        throw new ArgumentOutOfRangeException(nameof(index), $"Invalid index for {nameof(T)}.");
    }
}
