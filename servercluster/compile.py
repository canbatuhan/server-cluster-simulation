import os

SRC_PATH = "./src"
OBJ_PATH = "./obj/runner.exe"

def __generate_command(src_path:str, obj_path:str, cpp_files:list) -> str:
    words = list()
    words.append("g++")
    for each in cpp_files:
        words.append(os.path.join(src_path, each))
    words.append("-o")
    words.append(obj_path)
    return " ".join(words)

if __name__ == "__main__":
    cpp_files = list()
    for file_name in os.listdir(SRC_PATH):
        if file_name.find(".cpp") != -1:
            cpp_files.append(file_name)
    compile_cmd = __generate_command(
        SRC_PATH, OBJ_PATH, cpp_files)
    os.system(compile_cmd)
