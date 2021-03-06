#include "header.h"

using namespace delete_dubbels;
namespace fs = filesystem;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const string currentDateTime()
{
    time_t     now = time(nullptr);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "du    %Y-%m-%d    à    %X", &tstruct);
    return buf;
}

string delete_dubbels::ask_particular_path(string message_to_show)
{
    string chemin;
    std::error_code ec; // For using the non-throwing overloads of functions below.
    do
    {
        cout << endl << message_to_show;
        cin >> chemin;
        const fs::path path(chemin); // Constructing the path from a string is possible.
        if (fs::is_directory(path, ec))
        {
            cout << "Ce chemin a bien été reconnu comme étant le chemin vers un dossier : \n";
            cout << chemin << endl;
        }
        if (ec) // Optional handling of possible errors.
            std::cerr << "Error in is_directory: " << ec.message();
        if (fs::is_regular_file(path, ec))
        {
            cout << "Ce chemin a été reconnu comme étant le chemin vers un fichier !\n";
            cout << chemin;
        }
        if (ec) // Optional handling of possible errors. Usage of the same ec object works since fs functions are calling ec.clear() if no errors occur.
            std::cerr << "Error in is_regular_file: " << ec.message();
    } while (!fs::is_directory(chemin, ec));
    return chemin;
}

bool delete_dubbels::confirm_delete_all_dubbels_in_directory(string path)
{
    string answer;
    cout << endl << confirm_message << end << "' dans le dossier : " << endl;
    cout << path << endl;
    do
    {
        cout << endl << confirm_options;
        cin >> answer;
        std::transform(answer.begin(), answer.end(), answer.begin(), [](unsigned char c){
            return std::tolower(c);
        });
        if (answer == "oui"
                || answer == "o"
                || answer == "non"
                || answer == "n")
            break;
    } while (1);
    if (answer == "oui"
            || answer == "o")
        return true;
    else if (answer == "non"
             || answer == "n")
        return false;
    else
        return false;
}

unsigned long delete_dubbels::get_last_slash_position(string path)
{
    unsigned long slash_position = 0;
    for (unsigned long i = 0; i < path.length(); i++)
        if (path[i] == '/')
            slash_position = i;
    return slash_position;
}

string delete_dubbels::get_file_format(string path)
{
    string format = "";
    unsigned long slash_pos = get_last_slash_position(path);
    for (unsigned long i = path.length() - 1; i > slash_pos; i--)
    {
        format += path[i];
        if (path[i] == '.')
            break;
    }
    // Reverse str[begin..end]
    reverse(format.begin(), format.end());
    if (format[0] == '.')
        return format;
    else
        return "No format found";
}

bool delete_dubbels::has_correct_end(string path)
{
    string initial_path = path;
    if (delete_dubbels::get_file_format(path)[0] == '.')
    {
        unsigned long format_file_length = delete_dubbels::get_file_format(initial_path).length();
        for (unsigned j = 0; j < format_file_length; j++)
            initial_path.erase(
                        remove(
                            initial_path.begin() + static_cast<long>(initial_path.length()) - 1 - j,
                            initial_path.end(),
                            initial_path[initial_path.length() - 1]),
                    initial_path.end()
                    );
        for (unsigned long i = initial_path.length() - 1; i > initial_path.length() - 1 - delete_dubbels::max_end_length; i--)
            for (unsigned long j = delete_dubbels::end.length() - 1; j >= 0; j--)
                if (initial_path[initial_path.length() - 1] != delete_dubbels::end[j]
                        || initial_path[initial_path.length() - 1 - j] != delete_dubbels::end[j - j])
                    return false;
                else
                    return true;
    }
    return false;
}

string get_file_name(string path)
{
    string file_format = get_file_format(path);
    for (unsigned int i = 0; i < file_format.length(); i++)
        path.pop_back();
    string file_name;
    for (unsigned long i = path.length(); i > 0; i--)
    {
        if (path[i] == '/')
            break;
        file_name += path[i];
    }
    reverse(file_name.begin(), file_name.end());
    return file_name;
}

string delete_dubbels::duplicate_file(string original_file_path)
{
    cout << "original_file_path = " << original_file_path << endl;
    ifstream source(original_file_path.c_str(), ios::binary);
    string file_name = get_file_name(original_file_path);
    string path_with_only_underscores = change_slashes_into_underscores(original_file_path);
    string file_format = get_file_format(original_file_path);
    for (unsigned int i = 0; i < file_format.length(); i++)
    {
        path_with_only_underscores.pop_back();
        original_file_path.pop_back();
    }
    for (unsigned int i = 0; i < file_name.length(); i++)
    {
        path_with_only_underscores.pop_back();
        original_file_path.pop_back();
    }
    original_file_path += '/';
    original_file_path += path_with_only_underscores;
    original_file_path += '_';
    original_file_path += file_name;
    original_file_path += file_format;
    {
        ofstream dest(original_file_path, ios::binary);
        dest << source.rdbuf();
        source.close();
        dest.close();
    }
    cout << "original_file_path = " << original_file_path << endl;
    return original_file_path;
}

void delete_dubbels::delete_file(string original_file_path, ofstream &log_file, string trash_path)
{
    int status = 0;
    if (log_file.is_open())
        log_file << size_to_string(original_file_path) << " bytes : " << original_file_path << endl; //store array contents to text file
    else
        cout << "\nError Occurred on file named '" << original_file_path << "'";
    try
    {
        // 1. Dupliquer le fichier dans son dossier et lui ajouter ' copie' à la fin
        string duplicated_file_path = duplicate_file(original_file_path);
        // 2. Déplacer la copie du fichier
        string file_name = get_file_name(duplicated_file_path);
        file_name += '_';
        file_name += get_file_name(original_file_path);
        file_name += get_file_format(original_file_path);
        string file_format = get_file_format(original_file_path);
        trash_path += '/';
        for (unsigned int i = 0; i < file_name.length(); i++)
        {
            trash_path += file_name[i];
        }
        char * s = new char[strlen(trash_path.c_str())+strlen(original_file_path.c_str())+1];
        strcpy(s,trash_path.c_str());
        strcat(s, get_file_format(original_file_path).c_str());
        rename(duplicated_file_path.c_str(), s);
        status = remove(original_file_path.c_str());
    } catch (std::filesystem::filesystem_error& e) {
        std::cout << e.what() << '\n';
    }
    if(status == 0)
        cout<<"\nFile deleted successfully : " << original_file_path;
    else
        cout<<"\nError Occurred on file named '" << original_file_path << "'";
    cout << endl;
}

void delete_dubbels::recur(char* s, ofstream *log_file, string trash_path)
{
    char path[4096];
    struct dirent *lecture;
    DIR *repertoire;

    repertoire = opendir(s);
    // L'erreur est gérée dans la fonction confirm_delete_all_dubbels_in_directory(), du coup, ce if n'est pas nécessaire
    if (repertoire == nullptr)
    {
        perror(s);
        return;
    }
    //printf("%s:\n", s);
    while ((lecture = readdir(repertoire)) != nullptr)
    {
        if (lecture->d_type == DT_DIR)
        {
            if (strcmp(lecture->d_name, ".")!=0 && strcmp(lecture->d_name, "..")!=0)
            {
                strcpy(path, s);
                strcat(path,"/");
                delete_all_correct_files_in_dir(path, log_file, trash_path);
                strcat(path, lecture->d_name);
                delete_all_correct_files_in_dir(path, log_file, trash_path);
                recur(path, log_file, trash_path);
            }
        }
        else
        {
            //printf("->%s\n", lecture->d_name);
        }
    }
    closedir(repertoire);
}

void delete_dubbels::delete_all_correct_files_in_dir(string folder_path, ofstream *log_file, string trash_path)
{
    for (const auto & entry : fs::directory_iterator(folder_path))
        if (delete_dubbels::has_correct_end(entry.path()))
            delete_file(entry.path(), *log_file, trash_path);
}

string log_file_name(string path, string log_file_dir_path)
{
    std::error_code ec; // For using the non-throwing overloads of functions below.
    if (fs::is_directory(path, ec))
    {
        log_file_dir_path += "/";
        log_file_dir_path += path;
    }
    else
        cout << "Ceci n'est pas un répertoire : " << log_file_dir_path;
    return log_file_dir_path;
}

string delete_dubbels::change_slashes_into_underscores(string path)
{
    string new_path = path;
    for (unsigned int i = 0; i < new_path.length(); i++)
        if (new_path[i] == '/')
            new_path[i] = '_';
    return new_path;
}

string delete_dubbels::create_particular_trash_dir()
{
    fs::path trash_path = delete_dubbels::ask_particular_path("Entrez le chemin du dossier"
                                                              "dans lequel vous voulez créer le dossier qui contiendra tous les fichiers à supprimer.\n"
                                                              "Le dossier sera enregistré dans le dossier parent de celui que vous entrez : ");
    trash_path = trash_path.parent_path();
    trash_path += "/DELETED_FILES_PATHS";
    filesystem::create_directory(trash_path);
    return trash_path;
}

int delete_dubbels::get_file_size_int(string file_path)
{
    ifstream in_file(file_path, ios::binary);
    in_file.seekg(0, ios::end);
    long long file_size = in_file.tellg();
    cout << "file_size : " << file_size << endl;
    return static_cast<int>(file_size);
}

int delete_dubbels::total_freed_space(string file_path)
{
    int file_size = get_file_size_int(file_path);
    freed_space += file_size;
    return file_size;
}

string delete_dubbels::size_to_string(string file_path)
{
    string file_size_string = "";
    file_size_string += to_string(total_freed_space(file_path));
    for (unsigned long i = 0; i < max_file_size_string_length - file_size_string.length(); i++)
        file_size_string += ' ';
    return file_size_string;
}

void delete_dubbels::start()
{
    cout << start_message << "'" << end << "'" << endl;
    string path = delete_dubbels::ask_particular_path("Entrez le chemin du dossier dans lequel vous voulez supprimer les fichiers concernés : ");
    char* char_arr;
    char_arr = &path[0];
    string asked_log_file_path = delete_dubbels::ask_particular_path("Entrez un chemin vers un dossier"
                                                                     "dans lequel vous voulez stocker le fichier des logs de suppression : ");
    string trash_path = create_particular_trash_dir();
    string log_file_path_and_name = "";
    for (unsigned i = 0; i < asked_log_file_path.length(); i++)
        log_file_path_and_name += asked_log_file_path[i];
    asked_log_file_path += "/DELETED_FILES_PATHS.txt";
    ofstream log_file;
    log_file.open(asked_log_file_path, ios_base::app);
    log_file << endl << endl << "Nouvelles suppressions datant " << currentDateTime() << " :" << endl << endl;
    if (delete_dubbels::confirm_delete_all_dubbels_in_directory(path))
    {
        delete_dubbels::recur(char_arr, &log_file, trash_path);
        cout << end_message << path << endl;
    }
    else
        cout << nothing_to_do_message << endl;
    cout << freed_space << " bytes seront libérés de votre ordinateur lorsque vous supprimerez le dossier " << endl << trash_path << endl;
    log_file << endl << freed_space << " bytes seront libérés de votre ordinateur lorsque vous supprimerez le dossier " << endl << trash_path << endl;
    cout << endl;
    log_file.close();
}

void delete_dubbels::count_execution_time()
{
    // Use auto keyword to avoid typing long
    // type definitions to get the timepoint
    // at this instant use function now()
    auto startingProgramTime = chrono::high_resolution_clock::now();

    start();

    // After function call
    auto stoppingProgramTime = chrono::high_resolution_clock::now();

    // Subtract stop and start timepoints and
    // cast it to required unit. Predefined units
    // are nanoseconds, microseconds, milliseconds,
    // seconds, minutes, hours. Use duration_cast()
    // function.
    auto duration = chrono::duration_cast<chrono::microseconds>(stoppingProgramTime - startingProgramTime);

    // To get the value of duration use the count()
    // member function on the duration object
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
}

