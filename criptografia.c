#include <stdio.h>
#include <gtk/gtk.h>

void encrypt_file(const char* filename) {
    FILE* file = fopen(filename, "r+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    int shift = 3;

    int c;
    while ((c = fgetc(file)) != EOF) {
        if (c >= 'a' && c <= 'z') {
            c = 'a' + (c - 'a' + shift) % 26;
        } else if (c >= 'A' && c <= 'Z') {
            c = 'A' + (c - 'A' + shift) % 26;
        }
        fseek(file, -1, SEEK_CUR);
        fputc(c, file);
        fflush(file);
    }

    fclose(file);
    printf("Arquivo criptografado com sucesso!\n");
}

void on_file_selection_button_clicked(GtkWidget* widget, gpointer data) {
    GtkWidget* file_dialog;
    file_dialog = gtk_file_chooser_dialog_new("Selecione um arquivo", GTK_WINDOW(data), GTK_FILE_CHOOSER_ACTION_OPEN,
                                              "Cancelar", GTK_RESPONSE_CANCEL, "Abrir", GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(file_dialog)) == GTK_RESPONSE_ACCEPT) {
        char* filename;
        GtkFileChooser* file_chooser = GTK_FILE_CHOOSER(file_dialog);
        filename = gtk_file_chooser_get_filename(file_chooser);

        encrypt_file(filename);

        g_free(filename);
    }

    gtk_widget_destroy(file_dialog);
}

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget* window;
    GtkWidget* button;
    GtkWidget* vbox;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    button = gtk_button_new_with_label("Selecionar arquivo");
    g_signal_connect(button, "clicked", G_CALLBACK(on_file_selection_button_clicked), window);
    gtk_box_pack_start(GTK_BOX(vbox), button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
