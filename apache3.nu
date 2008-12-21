(load "template")

(set template
<<-END
<h1>Hello Number Three</h1>
END)

(eval (NuTemplate codeForString:template))

