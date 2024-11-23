import glob
import os.path
import sys


class cli_args:
    name = ""
    dest_path = ""
    source_dest_path = ""
    plugin_type = ""
    gui_backend = ""

    def __init__(self, _name, _dest_path, _plugin_type, _gui_backend):
        self.name = _name
        self.dest_path = _dest_path
        self.source_dest_path = os.path.join(self.dest_path, "source")
        self.plugin_type = _plugin_type
        self.gui_backend = _gui_backend


def exit_with_message(message: str):
    print(message)
    exit(-1)


def parse_args(argv):
    num_args: int = len(argv)
    if num_args < 3:
        exit_with_message(
            "Incorrect number of args passed.\nUsage: python3 create.py name dest type [gui_backend]")
    name = sys.argv[1]
    dest = sys.argv[2]
    plugin_type = sys.argv[3]
    gui_backend = ""
    if num_args > 4:
        gui_backend = sys.argv[4]
    return cli_args(_name=name, _dest_path=dest, _plugin_type=plugin_type, _gui_backend=gui_backend)


def process_file(args: cli_args, to_load: str):
    if not os.path.isfile(to_load):
        exit_with_message(to_load + " not found, aborting!")
    stripped_name = os.path.basename(to_load)
    dest_file = ""
    if stripped_name != "CMakeLists.txt":
        new_name = args.name.lower() + "_" + stripped_name
        dest_file = os.path.join(args.source_dest_path, new_name)
    else:
        dest_file = os.path.join(args.dest_path, "CMakeLists.txt")

    has_gui_str = "false" if args.gui_backend == "" else "true"
    if args.plugin_type == "fx":
        au_subtype = "aufx"
        clap_features = "audio_effect"
    elif args.plugin_type == "instrument":
        au_subtype = "aumu"
        clap_features = "instrument"
    else:
        exit_with_message("Invalid arg passed to type, options are fx or instrument: " + args.plugin_type)
    prepro_name = args.name.replace("-", "_").upper()

    with open(to_load, 'r') as source_file:
        contents = source_file.read()
        contents = contents.replace("$PREPRO_NAME$", prepro_name)
        contents = contents.replace("$LOWER_PROJECT_NAME$", args.name.lower())
        contents = contents.replace("$PROJECT_NAME$", args.name)
        contents = contents.replace("$HAS_GUI$", has_gui_str)
        contents = contents.replace("$AU_SUBTYPE$", au_subtype)
        contents = contents.replace("$CLAP_FEATURES$", clap_features)

        with open(dest_file, 'w') as new_file:
            new_file.write(contents)
            new_file.flush()


args = parse_args(sys.argv)
os.mkdir(args.dest_path)
os.mkdir(args.source_dest_path)
boilerplate_path = os.path.join(os.path.dirname(__file__), 'boilerplate')
gui_dependant_path = os.path.join(boilerplate_path, 'no_gui')
if args.gui_backend != "":
    if args.gui_backend.lower() == "web":
        gui_dependant_path = os.path.join(boilerplate_path, "web_gui")
    elif args.gui_backend.lower() == "raw":
        gui_dependant_path = os.path.join(boilerplate_path, "raw_gui")
    else:
        exit_with_message("Invalid gui backend provided, options are web or raw")
for file in glob.glob(gui_dependant_path + "/*.*", recursive=False):
    process_file(args, file)
for file in glob.glob(boilerplate_path + "/*.*", recursive=False):
    process_file(args, file)
