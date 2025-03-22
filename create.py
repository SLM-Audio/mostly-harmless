from argparse import ArgumentParser
import shutil
import sys
import os


arg_parser = ArgumentParser(
    prog='mostly harmless project creator',
    description='Create a new mostly harmless plugin project from a boilerplate project',
)

arg_parser.add_argument('name', help='Name of your plugin')
arg_parser.add_argument('path', help='Destination directory for your plugin')
arg_parser.add_argument('type', choices=['fx', 'instrument'], help='Type of plugin')
arg_parser.add_argument('--gui', help='GUI backend ("web", "raw", or "none")', default='none')


def main():    
    args = arg_parser.parse_args()
    
    if args.gui not in ['web', 'raw', 'none']:
        print('Error: --gui must be "web", "raw", or "none"')
        sys.exit(1)
        
    has_gui = 'false' if args.gui == 'none' else 'true'
    gui = args.gui if args.gui != 'none' else 'no'

    dest_path = args.path
    dest_src_path = os.path.join(dest_path, 'source')

    if os.path.isdir(dest_path):
        print(f'Error: {dest_path} already exists')
        sys.exit(1)

    try:
        os.mkdir(dest_path)
        os.mkdir(dest_src_path)

        prepro_name = args.name.replace('-', '_').upper()
        au_subtype, clap_features = ('aufx', 'audio_effect') if args.type == 'fx' else ('aumu', 'instrument')
        
        boilerplate_source = os.path.join(os.path.dirname(__file__), 'boilerplate')
        gui_source = os.path.join(boilerplate_source, f'{gui}_gui')

        files = [
            os.path.join(boilerplate_source, f) for f in os.listdir(boilerplate_source)
            if os.path.isfile(os.path.join(boilerplate_source, f))
        ]
        files.extend([
            os.path.join(gui_source, f) for f in os.listdir(gui_source)
            if os.path.isfile(os.path.join(gui_source, f))
        ])

        for file in files:
            with open(file, 'r') as f:
                contents = f.read().replace('$PREPRO_NAME$', prepro_name)   \
                    .replace("$LOWER_PROJECT_NAME$", args.name.lower())     \
                    .replace("$PROJECT_NAME$", args.name)                   \
                    .replace("$HAS_GUI$", has_gui)                          \
                    .replace("$AU_SUBTYPE$", au_subtype)                    \
                    .replace("$CLAP_FEATURES$", clap_features)
            
            stripped_name = os.path.basename(file)
            if stripped_name == 'CMakeLists.txt':
                dest_file = os.path.join(dest_path, stripped_name)
            else:
                stripped_name = f'{args.name.lower()}_{stripped_name}'
                dest_file = os.path.join(dest_src_path, stripped_name)
            
            with open(dest_file, 'w+') as f:
                f.write(contents)
    except Exception as e:
        print(e)
        if os.path.isdir(dest_path):
            shutil.rmtree(dest_path)        


if __name__ == '__main__':
    main()
