void create_ext_ent(LabelTable *labelTable, char *filename)
{
    FILE *ent_file; /* Write to ent file */
    FILE *ext_file; /* Write to ext file */

    char filename_ent[LABEL_LENGTH];
    char filename_ext[LABEL_LENGTH];
    strcpy(filename_ent, filename);
    strcpy(filename_ext, filename);

    strcat(filename_ent, ".ent"); /* Add file extension .ent to the original file name */
    ent_file = fopen(filename_ent, "w");

    strcat(filename_ext, ".ext"); /* Add file extension .ext to the original file name */
    ext_file = fopen(filename_ext, "w");

    Label *currentLabel = labelTable->head;

    while (currentLabel)
    {
        /* Print to ext file base & offset of external labels */
        if (currentLabel->external_flag == 1)
        {
            fprintf(ext_file, "%s base %d\n", currentLabel->name, currentLabel->value - (currentLabel->value) % 16);
            fprintf(ext_file, "%s offset %d\n", currentLabel->name, (currentLabel->value) % 16);
        }

        /* Print to ent file line number of entry symbols */
        if (currentLabel->entry_flag == 1)
        {
            fprintf(ent_file, "%s,%d,%d \n", currentLabel->name, currentLabel->value - (currentLabel->value) % 16, (currentLabel->value) % 16);
        }

        currentLabel = currentLabel->next;
    }

    fclose(ext_file);
    fclose(ent_file);
}
