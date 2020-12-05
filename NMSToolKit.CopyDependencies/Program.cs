using System.IO;

namespace NMSToolKit.CopyDependencies
{
    class Program
    {
        static void Main(string[] args)
        {
            foreach (var file in new string[]
            {
                "Magnum-d.dll",
                "MagnumGL-d.dll",
                "MagnumDebugTools-d.dll",
                "MagnumMeshTools-d.dll",
                "MagnumOpenDdl-d.dll",
                "MagnumPrimitives-d.dll",
                "MagnumSceneGraph-d.dll",
                "MagnumShaders-d.dll",
                "MagnumTrade-d.dll",
                "MagnumSceneGraph-d.dll",
                "CorradePluginManager-d.dll",
                "CorradeUtility-d.dll",
                "jpeg62.dll",
                "libpng16d.dll",
                "SDL2d.dll",
                "zlibd1.dll",
                "simdjson.dll"
            }) File.Copy(Path.Combine(args[0], file), Path.Combine(args[1], file), true);   
        }
    }
}
