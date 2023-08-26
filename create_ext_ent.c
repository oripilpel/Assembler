void create_ext_ent(LabelTable labelTable, char input_file_original[])
{
    FILE *ptr_file_write3; /* Write to ent file */
    FILE *ptr_file_write2; /* Write to ext file */

    char file_ent[LABEL_LENGTH];
    char file_ext[LABEL_LENGTH];
    strcpy(file_ent, input_file_original);
    strcpy(file_ext, input_file_original);

    strcat(file_ent, ".ent"); /* Add file extension .ent to the original file name */
    ptr_file_write3 = fopen(file_ent, "w");

    strcat(file_ext, ".ext"); /* Add file extension .ext to the original file name */
    ptr_file_write2 = fopen(file_ext, "w");

    Label *currentLabel = labelTable.head;

    while (currentLabel)
    {
        /* Print to ext file base & offset of external labels */
        if (currentLabel->external_flag == 1)
        {
            fprintf(ptr_file_write2, "%s base %d\n", currentLabel->name, currentLabel->value - (currentLabel->value) % 16);
            fprintf(ptr_file_write2, "%s offset %d\n", currentLabel->name, (currentLabel->value) % 16);
        }

        /* Print to ent file line number of entry symbols */
        if (currentLabel->entry_flag == 1)
        {
            fprintf(ptr_file_write3, "%s,%d,%d \n", currentLabel->name, currentLabel->value - (currentLabel->value) % 16, (currentLabel->value) % 16);
        }

        currentLabel = currentLabel->next;
    }

    fclose(ptr_file_write2);
    fclose(ptr_file_write3);
}
