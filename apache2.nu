(load "template")

(set template
<<-END
<h1>Hello Number Two</h1>
END)

(eval (NuTemplate codeForString:template))

