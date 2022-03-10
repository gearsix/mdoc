#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <cmark.h>


void usage()
{
	puts("usage: ./mdoc TITLE AUTHOR < file.md > file.html");
}

void printhead(const char *title, const char *author)
{
	puts("<!DOCTYPE html>");
	puts("<html lang=\"en\">");
	puts("<head>");
	puts("  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />");
	if (title && strlen(title) > 0)
		printf("  <title>%s</title>\n", title);
	if (title && strlen(author) > 0)
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

size_t cmark(const size_t bufsiz, FILE *in, FILE *out)
{
	char buf[bufsiz];
	size_t n = fread(buf, sizeof(char), BUFSIZ, in);
	char *html = cmark_markdown_to_html(buf, strlen(buf), 0);
	fprintf(out, "%s", html);
	free(html);
	return n;
}

void cleanexit() {
	printfoot();
	exit(SIGINT);
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

	void (*sig)(int) = 0;
	sig = signal(SIGINT, cleanexit);

	printhead(title, author);
	while (!sig && cmark(BUFSIZ, stdin, stdout) == BUFSIZ);
	printfoot();
}
