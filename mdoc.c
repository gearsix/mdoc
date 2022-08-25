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
	puts("</html>\0");
}

void cmark(FILE *in, FILE *out)
{
	const int opts = (CMARK_OPT_UNSAFE|CMARK_OPT_VALIDATE_UTF8);
	cmark_node *n = cmark_parse_file(in, opts);
	char *html = cmark_render_html(n, opts);
	fprintf(out, "%s", html);
	cmark_node_free(n);
	free(html);
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

	signal(SIGINT, cleanexit);

	setvbuf(stdout, NULL, _IONBF, 0);

	printhead(title, author);
	cmark(stdin, stdout);
	printfoot();
}
