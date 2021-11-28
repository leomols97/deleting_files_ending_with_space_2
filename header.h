#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <filesystem>
#include <iostream>
#include <cctype>
#include <string>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <locale> //instruction pré-processeur
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <chrono>

#include <QBuffer>
#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QtWidgets>

using namespace std;

namespace delete_dubbels
{
    /**
     * @brief end The end of the names of the files to delete
     */
    inline string end = " 2";
    inline string start_message = "Bienvenue dans le programme de suppression de fichier ";
    inline string end_message = "\n\nPlus aucun fichier se terminant par '" + end + "' ne se trouve dans le dossier :\n";
    inline string nothing_to_do_message = "Rien ne sera donc supprimé ! A bientôt...";
    inline string confirm_message = "Voulez-vous supprimer TOUTES les itérations de fichiers 'doublons' se terminant par '";
    inline string confirm_options = "Entrez [O] pour 'Oui' et [N] pour 'Non' : ";

    inline unsigned long max_end_length = end.length();

    /**
     * @brief max_file_size_string_length The maximum size of the weight of the size
     */
    inline unsigned long max_file_size_string_length = 20;

    /**
     * @brief freed_space The freed space to show in the log file and at the end of the execution
     */
    inline int freed_space = 0;

    /**
     * @brief file_size The size of the current file that will be deleted
     */
    inline int file_size = 0;



    /**
     * @brief ask_particular_path   Asks the user a path to a folder
     * @param message_to_show       The message to show to the user
     * @return                      The path the user entered
     */
    string ask_particular_path(string message_to_show);

    /**
     * @brief confirm_delete_all_dubbels_in_directory   Asks a confirmation of the deleting process to the user
     * @param path                                      The path the user wants to delete all the files in
     * @return                                          True if he confirms and false else
     */
    bool confirm_delete_all_dubbels_in_directory(string path);

    /**
     * @brief get_last_slash_position   Gets the position of the last '/' in a path (that is a string)
     * @param path                      The path in which looking for the '/'
     * @return                          The position of the '/'
     */
    unsigned long get_last_slash_position(string path);

    /**
     * @brief recur         Iterates recursively over the folders contained by a folder
     * @param s             The tab of chars of the path to iterate in
     * @param log_file      The logfile to write the paths of the deleted files in
     * @param trash_path    The path of the folder the program has to put all the files to delete in
     */
    void recur(char* s, ofstream *log_file, string trash_path);

    /**
     * @brief get_file_format   Gets the format o extension of a file
     * @param path              The path of the file to find the format or extension of
     * @return                  The format of the file
     */
    string get_file_format(string path);

    /**
     * @brief has_correct_end   Checks if a file has the correct end belonging the end it should have to be deleted
     * @param path              The path of the file to check
     * @return                  True if the file has the correct end and could be deleted and false else
     */
    bool has_correct_end(string path);

    /**
     * @brief delete_file           Deletes a files by putting it in the folder the user asked to put it in
     * @param original_file_path    The original path of the file
     * @param log_file              The logfile to write the paths of the deleted files in
     * @param trash_path            The path of the folder the program has to put all the files to delete in
     */
    void delete_file(string original_file_path, ofstream &log_file, string trash_path);

    /**
     * @brief write_path_of_deleted_files_into_log_file     Writes the path of the deleted file into the log file
     * @param log_file                                      The logfile to write the paths of the deleted files in
     */
    void write_path_of_deleted_files_into_log_file(string log_file_path);

    /**
     * @brief delete_all_correct_files_in_dir   Deletes all the files which the name finishes with the correct end
     * @param folder_path                       The path of folder to delete the files in
     * @param log_file                          The logfile to write the paths of the deleted files in
     * @param trash_path                        The path of the folder the program has to put all the files to delete in
     */
    void delete_all_correct_files_in_dir(string folder_path, ofstream *log_file, string trash_path);

    /**
     * @brief create_particular_trash_dir   Creates a folder that will contain all the deleted files in the parent folder that the user gives
     * @return                              The path of the trash folder
     */
    string create_particular_trash_dir();

    /**
     * @brief change_slashes_into_underscores   Changes every '/' into '_' in a path
     * @param path                              The path to modify
     * @return                                  The new 'path'
     */
    string change_slashes_into_underscores(string path);

    /**
     * @brief duplicate_file        Duplicates the file at the place it is in
     * @param original_file_path    The path of the file to duplicate
     * @return                      The path received in parametes but modified
     */
    string duplicate_file(string original_file_path);

    /**
     * @brief get_file_size_int     Gets the size of a file
     * @param file_path             The path to the file to check its size
     * @return                      The size of the file
     */
    int get_file_size_int(string file_path);

    /**
     * @brief total_freed_space     Computes the total freed space when all the files will be deleted
     * @param file_path             The path of the file to add its size to the total
     * @return                      The total freed space
     */
    int total_freed_space(string file_path);

    /**
     * @brief total_freed_space     Computes the total freed space when all the files will be deleted
     * @param file_path             The path of the file to add its size to the total
     * @return                      A string with the size of the file given as parameter
     */
    string size_to_string(string file_path);

    /**
     * @brief start     Starts the program
     */
    void start();

    /**
     * @brief count_execution_time      Computes the time of execution of the deletion
     *                                  by calling 'start()' function and stopping the time after 'start()' finishes
     */
    void count_execution_time();
}

#endif // HEADER_H
