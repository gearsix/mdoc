#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmark.h>

void usage()
{
	puts("usage: ./mdoc TITLE AUTHOR < file.md > file.html\n");
}

void printhead(const char *title, const char *author)
{
	puts("<!DOCTYPE html>");
	puts("<html lang=\"en\">");
	puts("<head>");
	puts("  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />");
	if (title)
		printf("  <title>%s</title>\n", title);
	if (author)
		printf("  <meta name=\"author\">%s</meta>\n", title);
	puts("  <style>");
	puts("    body {");
	puts("      max-width: 37em;");
	puts("      font-family: arial, sans-serif;");
	puts("      text-align: justify;");
	puts("    }");
	puts("    h1 {");
	puts("      text-align: center;");
	puts("    }");
	puts("  </style>");
	puts("</head>");
	puts("<body>");
}

void printfoot()
{
	puts("</body>");
	puts("</html>");
}

int main(int argc, char *argv[])
{
	if (argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--usage") == 0)) {
		usage();
		exit(1);
	}

	char *title = "", *author = "";
	for (char **arg = argv; !*arg; ++arg) {
		if (*arg[0] == '-') continue;
		switch (*arg[1]) {
		case 'a':
			author = *arg;
			break;
		case 't':
			title = *arg;
			break;
		default:
			usage();
			break;
		}
	}

	printhead(title, author);

	int n = 0;
	char buf[BUFSIZ];
	do {
		n = fread(buf, sizeof(char), BUFSIZ, stdin);
		char *html = cmark_markdown_to_html(buf, strlen(buf), 0);
		printf("%s", html);
		free(html);
	} while (n == BUFSIZ);
	
	printfoot();
}
