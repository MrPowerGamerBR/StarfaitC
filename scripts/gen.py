import sys
from pathlib import Path

# POOR MANS' GENERIC GENERATION THINGY
# "Because macros are sometimes kinda sad" -MrPowerGamerBR

print(sys.argv)

templatesFolder = Path(sys.argv[1])
generatedDirectory = Path(sys.argv[2])

# Don't forget to update the CMakeLists to include the new files!
arrayListVariants = [
    {
        "file": "arraylist_uint32",
        "name": "Uint32ArrayList",
        "type": "uint32_t",
        "includes": []
    },
    {
        "file": "arraylist_builtinfunction",
        "name": "BuiltinFunctionArrayList",
        "type": "BuiltinFunction",
        "includes": ["\"vm/builtinfunction.h\""]
    },
    {
        "file": "arraylist_variable",
        "name": "VariableArrayList",
        "type": "Variable",
        "includes": ["\"wad/variable.h\""]
    }
]

with open(templatesFolder / "arraylist.h", "r", encoding="utf-8") as f:
    contents = f.read()

    for arrayListVariant in arrayListVariants:
        includes = ""
        for include in arrayListVariant["includes"]:
            includes = includes + "#include " + include + "\n"

        with open(generatedDirectory / (arrayListVariant["file"] + ".h"), "w") as f:
            f.write(
                ("// BEEP BOOP THIS WAS AUTOMATICALLY GENERATED - DO NOT EDIT MANUALLY!!\n" + contents)
                    .replace("__ARRAY_LIST_NAME__", arrayListVariant["name"])
                    .replace("__ARRAY_LIST_TYPE__", arrayListVariant["type"])
                    .replace("__INCLUDES__", includes)
                )